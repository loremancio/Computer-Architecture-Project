SRC_DIR := src
BIN_DIR := binaries

.PHONY: all usage clean

all: par parO2 parM parMO2

par: $(SRC_DIR)/par.cpp
	g++ $< -o $(BIN_DIR)/par -lrt -pthread
	@echo "par compiled"

parO2: $(SRC_DIR)/par.cpp
	g++ $< -o $(BIN_DIR)/parO2 -lrt -pthread -O2
	@echo "parO2 compiled"

parM: $(SRC_DIR)/parM.cpp
	g++ $< -o $(BIN_DIR)/parM -lrt -pthread
	@echo "parM compiled"

parMO2: $(SRC_DIR)/parM.cpp
	g++ $< -o $(BIN_DIR)/parMO2 -lrt -pthread -O2
	@echo "parMO2 compiled"

clean:
	rm -f $(BIN_DIR)/*

usage:
	@echo "Usage: make [target]"
	@echo "Available targets:"
	@echo "    all    			: Compile all targets"
	@echo "    clean  			: Remove compiled binaries"
	@echo "    [par, parO2, parM, parMO2] 	: Compile specific target"

.DEFAULT_GOAL := usage
