#

PROJECT_ID = fs
ARGS = tests/1.txt

CC = gcc
CFLAGS = -I$(SRC_DIR) -Werror -Wextra

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TARGET = $(BIN_DIR)/$(PROJECT_ID)

.PHONY: asan clean run run_asan

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

asan: CFLAGS += -fsanitize=address
asan: $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: $(TARGET)
	$(TARGET) $(ARGS)

run-asan: clean asan
	$(TARGET) $(ARGS)
