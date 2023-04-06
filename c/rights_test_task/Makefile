

# CFLAGS = -Wall -Werror
CFLAGS = -Wall -g
BUILD_DIR = build
BIN_DIR = bin

OUT= $(BUILD_DIR)/$(BIN_DIR)/main_run

all: clean build

build: $(BUILD_DIR)/main.o
	$(info --- $@ ---)

	mkdir -p $(dir $(OUT))
	gcc $(CFLAGS) $^ -o $(OUT)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	gcc $(CFLAGS) $< -c -o $@

clean:
	rm -rvf $(BUILD_DIR)

