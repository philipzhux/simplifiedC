CXX       := g++
CXX_FLAGS := -std=c++11 -ggdb

BIN     := .
SRC     := .
INCLUDE := .

LIBRARIES   :=
EXECUTABLE  := test


all: $(BIN)/$(EXECUTABLE)

run: clean all
    clear
    ./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
    $(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
    -rm $(BIN)/*