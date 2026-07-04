BUILD_DIR := build
BIN := tlsock

.PHONY: all build run clean

all: build

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(BIN)

clean:
	rm -rf $(BUILD_DIR)
