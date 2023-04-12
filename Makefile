# Compiler
CXX = g++

# Include directories
INCLUDES = -I./parser_src/include -I./scanner_src/include

# Compiler flags
CXXFLAGS = -Wall -std=c++14 $(INCLUDES)

# Source files
SCANNER_SRC = ./scanner_src/dfa.cpp ./scanner_src/nfa.cpp ./scanner_src/scanner.cpp
BUILDER_SRC = ./parser_src/build.cpp ./parser_src/configuration.cpp ./parser_src/grammar.cpp ./parser_src/parser.cpp ./parser_src/production.cpp ./parser_src/symbol.cpp
PARSER_SRC = ./parser_src/main.cpp ./parser_src/configuration.cpp ./parser_src/grammar.cpp ./parser_src/parser.cpp ./parser_src/production.cpp ./parser_src/symbol.cpp

# Object files
BUILDER_OBJ = $(patsubst ./%,build/%,$(BUILDER_SRC:.cpp=.o))
PARSER_OBJ = $(patsubst ./%,build/%,$(PARSER_SRC:.cpp=.o))

# Executables
BUILDER_EXE = builder
PARSER_EXE = parser

all: $(PARSER_EXE) parser.xml

$(BUILDER_EXE): $(BUILDER_OBJ) $(SCANNER_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PARSER_EXE): $(PARSER_OBJ) $(SCANNER_SRC)
	$(CXX) $(CXXFLAGS) $(PARSER_OBJ) $(SCANNER_SRC) -o $@

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

parser.xml: $(BUILDER_EXE)
	./$(BUILDER_EXE) parser.xml

.PHONY: clean build_from_scratch clean-all

clean:
	rm -f $(BUILDER_OBJ) $(PARSER_OBJ) $(BUILDER_EXE) $(PARSER_EXE)

build_from_scratch: clean $(BUILDER_EXE) parser_xml

clean-all:
	rm -f $(BUILDER_OBJ) $(PARSER_OBJ) $(BUILDER_EXE) $(PARSER_EXE) parser.xml
