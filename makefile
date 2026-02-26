BUILD_DIR := build
ifeq ($(OS),Windows_NT)
    TARGET := output.exe
else
	TARGET := output
endif

.PHONY: all configure build run clean

all: build run

config:
	cmake -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Debug

build: config
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
