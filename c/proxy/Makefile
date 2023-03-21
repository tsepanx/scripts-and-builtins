

CFLAGS = -Wall -Werror
#CFLAGS = -Wall
BUILD_DIR = build

all: clean build

build: $(BUILD_DIR)/main.o $(BUILD_DIR)/utils.o
	gcc $(CFLAGS) $^ -o $(BUILD_DIR)/exec

$(BUILD_DIR)/main.o: main.c # main.h
	gcc $(CFLAGS) $< -c -o $@

$(BUILD_DIR)/utils.o: funcs.c funcs2.h
	gcc $(CFLAGS) $< -c -o $@

clean:
	rm -rvf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

single: single.c
	gcc single.c -o single
