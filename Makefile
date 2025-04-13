# Raylib 3D Modeler Makefile

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I./include $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

# Project files
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = ray3dproto

# Test files
TEST_DIR = tests
TEST_BUILD_DIR = $(BUILD_DIR)/tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c) $(wildcard $(TEST_DIR)/mocks/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(TEST_SRC))
TEST_TARGETS = $(TEST_BUILD_DIR)/test_world

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) $(SRC_DIR)/raylib_shim.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Rules
all: create_build_dir $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

create_build_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(TEST_BUILD_DIR)
	@mkdir -p $(TEST_BUILD_DIR)/mocks

# Code formatting
format:
	@echo "Formatting source code..."
	@find $(SRC_DIR) $(INCLUDE_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "Code formatting complete!"

# Test rules
test: create_build_dir $(OBJS) $(TEST_TARGETS)
	@echo "Running unit tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		$$test; \
		echo ""; \
	done

$(TEST_BUILD_DIR)/test_world: $(TEST_BUILD_DIR)/test_world.o $(TEST_BUILD_DIR)/mocks/raylib_mocks.o $(BUILD_DIR)/game_world.o
	$(CC) -o $@ $^ $(LDFLAGS) -lm

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Target for testing stl_load (includes main() when TEST_STL_LOAD is defined)
test_stl_load: $(SRC_DIR)/stl_load.c $(SRC_DIR)/stl_load.h
	$(CC) $(CFLAGS) -g -O0 -DTEST_STL_LOAD -I$(SRC_DIR) -o $@ $(SRC_DIR)/stl_load.c $(LDFLAGS)

# Target for compiling stl_load.o (object file without main)
stl_load.o: $(SRC_DIR)/stl_load.c $(SRC_DIR)/stl_load.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/stl_load.c -o stl_load.o

# Target for debugging stl_load (launch debugger on test_stl_load binary)
debug_stl_load: test_stl_load
	lldb ./test_stl_load

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_BUILD_DIR) test_stl_load stl_load.o

.PHONY: all clean create_build_dir test format 