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

gen-certs:
	openssl req -x509 -newkey rsa:2048 -keyout certs/key.pem -out certs/cert.pem -days 365 -nodes