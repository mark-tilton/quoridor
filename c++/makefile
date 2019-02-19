CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -O3

BIN		:= bin
SRC		:= src
INCLUDE	:= inc

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cc
	$(CXX) $^ $(CXX_FLAGS) -I$(INCLUDE) -lSDL2 -o $@

clean:
	-rm $(BIN)/*
