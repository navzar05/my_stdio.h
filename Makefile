CC = gcc
CFLAGS = -g -fPIC


SRC_DIR = src
BUILD_DIR = build


SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: build

build: $(OBJECTS)
	$(CC) -g -shared $^ -o ./$(BUILD_DIR)/libso_stdio.so


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


copy_to_test: build/libso_stdio.so
	cp -u build/libso_stdio.so ~/Desktop/checker-lin; \
	cp -u ~/Desktop/checker-lin/libso_stdio.so ~/Desktop/checker/checker-lin/


clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/libso_stdio.so
