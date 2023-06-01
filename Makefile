CC = g++
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
CFLAGS = -std=c++17 -I$(INCLUDE_DIR)

EXE_FILE = main.out

OBJECTS = $(BUILD_DIR)/network.o $(BUILD_DIR)/commandHandler.o $(BUILD_DIR)/main.o

all: $(BUILD_DIR) $(EXE_FILE)

$(EXE_FILE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXE_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/network.hpp $(INCLUDE_DIR)/commandHandler.hpp $(INCLUDE_DIR)/errors.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/commandHandler.o: $(SRC_DIR)/commandHandler.cpp $(INCLUDE_DIR)/commandHandler.hpp $(INCLUDE_DIR)/network.hpp $(INCLUDE_DIR)/errors.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/commandHandler.cpp -o $(BUILD_DIR)/commandHandler.o

$(BUILD_DIR)/network.o: $(INCLUDE_DIR)/network.hpp $(INCLUDE_DIR)/errors.hpp $(SRC_DIR)/network.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/network.cpp -o $(BUILD_DIR)/network.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out