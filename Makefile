BUILD_DIR := build

.PHONY: all test clean

all:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cmake --build $(BUILD_DIR)

test:
	cd $(BUILD_DIR) && ctest

clean:
	rm -rf $(BUILD_DIR)
