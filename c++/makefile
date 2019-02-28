CXX		       := g++
COMPILER_FLAGS := -Wall -Wextra -std=c++17 -ggdb -O2
LINKER_FLAGS   := -Iinc -I/usr/include/SDL2/ -lSDL2 -lSDL2_ttf

BIN		:= bin
SRC		:= src

EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cc
	mkdir -p $(@D)
	$(CXX) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

clean:
	-rm $(BIN)/*
