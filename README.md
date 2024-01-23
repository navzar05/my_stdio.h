-----------------------------Implementare stdio.h-----------------------------
------------------Descrierea pentru implementarea functiilor------------------

La baza implementarii, sta structura _so_file care are urmatoarea forma:

struct _so_file {
	int fd;
	int readBytes;
	char eof;
	char mode[2];
	unsigned char buffer[BUFSIZE];
	int index_buffer;
	unsigned char lastOperation;
	int error_code;
	int childPid;
};

fd - file descriptorul fisierului
readBytes - numarul total de bytes cititi de ultimul apel read()
eof - 1 daca s-a ajuns la finalul fisierului, 0 in caz contrar
mode - modul de acces
buffer - buffer de 4096 de octeti pentru a mentine numarul de apeluri de read si write cat mai mic
index_buffer - unde se afla "cursorul" pentru afisare/citire in buffer
lastOperation - 'r' daca ultima operatie a fost de citire, 'w' daca a fost de scriere
error_code - 0 daca nu a fost intampinata vreo eroare, diferita de 0 in caz contrar
childPid - pid-ul procesului copil creat prin apelul fork() in so_popen()

1. so_fopen()

Functia verifica intai corectitudinea argumentului mode.
Dupa alocarea si initializarea structurii, switch case-ul gestioneaza modul in care este deschis fisierul al carui nume a fost dat ca argument. Daca nu a aparut nicio eroare, intoarce pointer-ul catre structura alocata.

2. so_fclose()

Se verifica daca fisierul a fost deschis intr-unul din modurile in care este permisa scrierea; daca da, se executa so_fflush() pentru a afisa eventualele date ramase in buffer.
Se inchide file descriptorul si se elibereaza memoria.

3. so_fileno()

Intoarce valoarea lui fd din structura SO_FILE.

4. so_fflush()

Daca ultima operatie nu a fost una de scriere, executia nu are sens, astfel ca functia intoarce zero. In caz contrar, se afiseaza in fisier continutul buffer-ului dupa care acesta este golit.

5. so_fseek()

Daca ultima operatie a fost una de scirere, se executa so_fflush(); in caz contrar (ultima operatie a fost de citire), datele din buffer sunt invalidate. Se seteaza pozitia cursorului cu apelul functiei lseek() la pozitia dorita.

6. so_ftell()

Se salveaza in variabila pos pozitia curenta in fisier. Daca ultima operatie a fost de citire, se scade din aceasta diferenta dintre numarul total de octeti cititi de ultimul apel de read() si indexul din buffer.
Daca ultima operatie a fost de scriere, se aduna la variabila pos indexul buffer-ului. Aceste operatii sunt necesare pentru a returna pozitia reala a cursorului in fisier. Apelul lseek() nu este indestulator din cauza utilizarii buffer-ului si citirii in blocuri a cate 4096 de octeti.

7. so_fread()

Daca ultima operatie a fost una de scriere si modul permite o astfel de operatie, intai se executa so_fflush() pentru a garanta corectitudinea datelor.
Dupa aceasta operatie, se intra intr-o bucla for de la 0 la numarul total de octeti care sunt ceruti a fi cititi. Se apeleaza so_fgetc() pentru a citi octet cu octet datele din stream.
La final, functia intoarce numarul total de octeti cititi de catre functie.

8. so_fwrite()

Daca ultima operatie a fost una de citire si modul permite citirea si scrierea, datele din buffer sunt invalidate. Se intra intr-o bucla for asemanatoare cu cea din apelul so_fwrite() si se scriu datele de la adresa ptr folosind functia so_fputc().
La final, functia intoarce numarul total de octeti cititi de catre functie.

9. so_fgetc()

Daca s-a consumat toata informatia din buffer, il golim si citim din fisier. Daca apelul read() esueaza, setam error_code cu o valoarea difertia de 0 si returnam SO_EOF. Daca nu se citeste nimic, setam eof la 1 si returnam SO_EOF. Returnam direct din buffer utilizand variabila result.

10. so_fputc()

Daca buffer-ul este plin, apelam so_fflush(). In rest, scriem in buffer valoarea data ca argument. Returneaza valoarea scrisa daca executia a reusit.

11. so_feof()

Returneaza valoarea variabilei eof din structura SO_FILE.

12. so_ferror()

Returneaza valoarea variabilei error_code din structura SO_FILE.

13. so_popen()

Dupa initializarea structurii, se creeaza un pipe anonim. Se apeleaza fork(); procesul copil inchide capetele pipe-ului pe care nu le utilizeaza, redirecteaza input-ul sau output-ul (in functie de modul dat ca argument functiei) si executa comanda data ca argument. La final, inchide capetele ramase deschise ale pipe-ului si isi incheie executia.
Procesul parinte inchide capetele pipe-ului pe care nu le utilizeaza, duplica descriptorul pipe-ului pe care il foloseste in variabila fd din structura SO_FILE si inchide si aceste capete ramase ale pipe-ului. De asemenea, procesul parinte retine in variabila childPid din structura SO_FILE pid-ul procesului copil creat. Intoarce structura SO_FILE creata in cazul in care nu s-au intampinat erori.

14. so_pclose()

Apeleaza so_fflush() pentru a se asigura ca, in cazul in care a fost apelata vreo operatie de scriere, datele au fost scrise in pipe. Dupa care inchide file descriptorul pentru a transmite procesului copil ca nu mai sunt date de citit/scris. Procesul parinte asteapta incheierea executiei. Daca a aparut o eroare, intoarce -1, in caz contrar, intoarce codul de iesire al procesului.
