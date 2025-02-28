CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

SRC_DIR = src
TESTS = tests
BUILD_DIR = build
APP_EXE = $(BUILD_DIR)/app.exe
UNIT_TESTS = $(BUILD_DIR)/unit-tests

UNIT_TESTS_DIR = $(TESTS)/unit
INTEG_TESTS_DIR = $(TESTS)/integration

BUILD_GTEST = $(BUILD_DIR)/gtest
GTEST_DIR ?= googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

PIP = pip3
PYTHON = python3
VENV_NAME = venv
VENV_BIN = $(VENV_NAME)/bin
VENV_PIP = $(VENV_BIN)/pip
VENV_PYTHON = $(VENV_BIN)/python
VENV_ACTIVATE = $(VENV_BIN)/activate
PYTEST = $(VENV_BIN)/pytest

FORMAT_DIRS = $(SRC_DIR) $(UNIT_TESTS_DIR)

$(shell mkdir -p build/gtest)
$(shell git clone https://github.com/google/googletest &> /dev/null)

.PHONY: all clean run-int run-float run-unit-test run-integration-tests

###################################
####### COMMANDS TO EXECUTE #######
###################################

all: $(APP_EXE) $(UNIT_TESTS)

clean:
	@rm -rf build/
	@rm -rf $(VENV_NAME)
	@rm -rf tests/integration/__pycache__
	@rm -rf .pytest_cache

run-int: $(APP_EXE)
	@$<

run-float: $(APP_EXE)
	@$< --float

run-unit-test: $(UNIT_TESTS)
	@build/unit-tests.exe

run-integration-tests: $(APP_EXE) install-deps
	. $(VENV_BIN)/activate; $(PYTEST) $(INTEG_TESTS_DIR)/integ_tests.py

format:
	@find $(FORMAT_DIRS) -type f \( \
		-name "*.cpp" -o \
		-name "*.c" -o \
		-name "*.h" \
	\) -exec clang-format -i -style=file {} +

###################################
############ BUILD APP ############
###################################

$(APP_EXE): src/main.c
	@echo "Building app.exe"
	@$(CC) $(CFLAGS) -o $(APP_EXE) src/main.c

####################################
# BUILD GOOGLE TEST STATIC LIBRARY #
####################################

# Google Test object files
$(BUILD_GTEST)/gtest-all.o: $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
	@echo "Building gtest libraries"
	@$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_GTEST)/gtest_main.o: $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
	@echo "Building gtest libraries"
	@$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc -o $@

# Google Test static libraries
$(BUILD_GTEST)/gtest_main.a: $(BUILD_GTEST)/gtest-all.o $(BUILD_GTEST)/gtest_main.o
	@echo "Unpacking gtest libraries"
	@ar rv $@ $^ -o $@

##################################
######## BUILD UNIT TESTS ########
##################################

$(UNIT_TESTS): build/unit-tests.exe

$(BUILD_DIR)/unit-tests.exe: $(BUILD_GTEST)/gtest_main.a build/app-test.o tests/unit/unit-tests.cpp
	@$(CXX) -isystem $(GTEST_DIR)/include -pthread \
		tests/unit/unit-tests.cpp \
		$(BUILD_GTEST)/gtest_main.a build/app-test.o \
		-o build/unit-tests.exe

$(BUILD_DIR)/app-test.o: src/main.c
	@$(CC) $(CFLAGS) -DGTEST -c src/main.c -o build/app-test.o -g

##################################
######## CONFIGURING VENV ########
##################################

install-deps: venv
	@echo "Installing dependencies..."
	@. $(VENV_ACTIVATE) && $(VENV_PIP) install --upgrade pip
	@. $(VENV_ACTIVATE) && $(VENV_PIP) install pytest

venv: check-venv
	@if [ ! -d "$(VENV_NAME)" ]; then \
		echo "Creating virtual environment..."; \
		$(PYTHON) -m venv $(VENV_NAME); \
	fi

check-venv:
	@$(PYTHON) -c "import venv" 2>/dev/null || (echo "python3-venv not found. Installing..." && sudo apt update && sudo apt install -y python3-venv)

