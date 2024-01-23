#include "../include/so_stdio.h"

#define READ_END 0
#define WRITE_END 1

SO_FILE *so_popen(const char *command, const char *type)
{
	SO_FILE *fp = (SO_FILE *)calloc(1, sizeof(SO_FILE));

	if (fp == NULL)
		return NULL;

	int _pipe[2] = {0};

	fp->readBytes = 0;

	fp->eof = 0;

	fp->index_buffer = 0;

	fp->lastOperation = 0;

	memset(fp->buffer, 0, BUFSIZE);
	memcpy(fp->mode, type, 1);

	fp->mode[1] = '\0';

	int retValuePipe = pipe(_pipe);
	int status = 0;

	if (retValuePipe < 0) {
		free(fp);
		return NULL;
	}

	int pidOfChild = fork();

	if (pidOfChild < 0) {
		free(fp);
		return NULL;
	} else if (pidOfChild == 0) {
		switch (type[0]) {
		case 'r':
			close(_pipe[READ_END]);
			dup2(_pipe[WRITE_END], STDOUT_FILENO);
		break;
		case 'w':
			close(_pipe[WRITE_END]);
			dup2(_pipe[READ_END], STDIN_FILENO);
		break;
		}

		if (execlp("sh", "sh", "-c", command, NULL) < 0)
			exit(-1);
		if (type[0] == 'r')
			close(_pipe[WRITE_END]);
		else
			close(_pipe[READ_END]);

		exit(0);
	} else if (pidOfChild > 0) {
		switch (type[0]) {
		case 'r':
			close(_pipe[WRITE_END]);
			fp->fd = dup(_pipe[READ_END]);
			close(_pipe[READ_END]);
		break;
		case 'w':
			close(_pipe[READ_END]);
			fp->fd = dup(_pipe[WRITE_END]);
			close(_pipe[WRITE_END]);
		break;
		}
	}
	fp->childPid = pidOfChild;
	return fp;
}
