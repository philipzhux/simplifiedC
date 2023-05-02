# CUHK Compiler Course Project: C to MIPS Compiler

This repository contains a C to MIPS compiler developed as a course project for the compiler course `CSC4180` at CUHK. The compiler translates a subset of the C language into MIPS assembly code, focusing on basic arithmetic, logical operations, control structures, and array operations.

## Project Structure

The project is organized into the following structure:

```
.
├── Makefile
├── ast (namespace ASTGen)
│   ├── code.cpp
│   ├── code.hpp
│   ├── symbol_table.cpp
│   ├── symbol_table.hpp
│   ├── syntax_tree.cpp
│   ├── syntax_tree.hpp
│   └── utils.hpp
├── parser
├── parser_src
│   ├── configuration.cpp
│   ├── grammar.cpp
│   ├── include
│   │   ├── configuration.h
│   │   ├── grammar.h
│   │   ├── parser.h
│   │   ├── production.h
│   │   └── symbol.h
│   ├── main.cpp
│   ├── parser.cpp
│   ├── production.cpp
│   └── symbol.cpp
├── readme.md
├── run_parser.sh
├── scanner_src
│   ├── Makefile
│   ├── README.md
│   ├── dfa.cpp
│   ├── include
│   │   ├── dfa.hpp
│   │   ├── nfa.hpp
│   │   └── scanner.hpp
│   ├── main.cpp
│   ├── nfa.cpp
│   ├── run_scanner.sh
│   └── scanner.cpp
```
### Grammar in CFG
![cfg_ss](https://i.imgur.com/X03xgEx.png)

### Components

*To understand the code, you can start with `grammar.cpp/h` and `syntax_tree.cpp/hpp`. You can refer to `parser.cpp/h` for parser and `scanner.cpp/h` for scanner.*

- `ast`: Contains the code for abstract syntax tree (AST) generation, symbol table management, and MIPS assembly code generation.
  - `code.cpp/hpp`: Encapsulates code generation small building blocks.
  - `symbol_table.cpp/hpp`: Contains the SymbolTable class for managing the symbol table during code generation.
  - `syntax_tree.cpp/hpp`: Defines the SyntaxTreeNode class and its generateCode() method for generating MIPS assembly code.
- `parser`: Contains the parser implementation, which builds the parsing table and parses the tokens.
  - `configuration.cpp/h`: Contains the Configuration class and ConfigurationSet class, representing a configuration (production rule plus dot; a configuration set is a state) in the parsing process.
  - `grammar.cpp/h`: Contains the Grammar class, which defines the grammar (production) rules for the parser, the semantic routines that trigger the code gen process are also attached to the production rules.
  - `parser.cpp/h`: Contains the Parser class, which is responsible for building the parsing table and parsing the tokens.
  - `production.cpp/h`: Contains the Production class, which is an abstraction for a production rule in the grammar.
  - `symbol.cpp/h`: Contains the Symbol class, which represents a grammar symbol.
- `scanner_src`: Contains the scanner implementation, which is responsible for scanning the input C code and converting it into tokens.
  - `dfa.cpp/hpp`: Contains the DFA class, which represents a deterministic finite automaton.
  - `nfa.cpp/hpp`: Contains the NFA class, which represents a nondeterministic finite automaton.
  - `scanner.cpp/hpp`: Contains the Scanner class, which implements the main scanning functionality, which utilizes nfa for regular expression matching

## Building the Compiler

To build the compiler, simply run `make` in the root directory of the project:

```
make
```

This will generate the `compiler` executable in the project root.

## Usage

To compile a C source file, run the following command:

```bash
./compiler  <  input_file.c  >  output_file.asm
```

This will generate a MIPS assembly file `output_file.asm` from the C source file `input_file.c`.

You can then run the MIPS assembly code using a MIPS simulator, such as [SPIM](http://spimsimulator.sourceforge.net/) or [MARS](http://courses.missouristate.edu/kenvollmar/mars/).

## Limitations

- The compiler currently only supports a single `main` function with no parameters and no return value.
- No support for pointers, structures, or other advanced C language features.
- No optimization is performed on the generated assembly code.

## Contributing

Contributions are welcome! If you find any bugs, have suggestions for new features, or would like to help improve the project, please feel free to open an issue or create a pull request.