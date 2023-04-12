# LR(1) Parser for a Simplified C Language

## Introduction
This report presents the design and implementation of an LR(1) parser for a simplified version of the C language. The parser is able to parse a given sequence of C tokens and produce the corresponding parse tree. This document will cover the LR(1) grammar, the process of building the LR(1) parsing table, and using the parsing table to parse a series of C tokens. Additionally, it will discuss the creation of two executable files, "builder" and "parser", and their respective roles in building and parsing the input.

## LR(1) Grammar
The LR(1) grammar for the simplified C language is presented below:

```Rust
[Production Rules]
program -> var_declarations statements

var_declarations -> var_declarations var_declaration
var_declarations -> var_declaration
var_declarations -> ε

var_declaration -> INT declaration_list SEMI

declaration_list -> declaration_list COMMA declaration
declaration_list -> declaration

declaration -> ID ASSIGN INT_NUM
declaration -> ID LSQUARE INT_NUM RSQUARE
declaration -> ID

code_block -> statement
code_block -> LBRACE statements RBRACE

statements -> statements statement
statements -> statement

statement -> assign_statement SEMI
statement -> control_statement
statement -> read_write_statement SEMI
statement -> SEMI

control_statement -> if_statement
control_statement -> while_statement
control_statement -> do_while_statement SEMI
control_statement -> return_statement SEMI

read_write_statement -> read_statement
read_write_statement -> write_statement

assign_statement -> ID LSQUARE exp RSQUARE ASSIGN exp
assign_statement -> ID ASSIGN exp

if_statement -> if_stmt
if_statement -> if_stmt ELSE code_block

if_stmt -> IF LPAR exp RPAR code_block

while_statement -> WHILE LPAR exp RPAR code_block

do_while_statement -> DO code_block WHILE LPAR exp RPAR

return_statement -> RETURN

read_statement -> READ LPAR ID RPAR

write_statement -> WRITE LPAR exp RPAR

exp -> exp1

exp1 -> exp1 OROR exp2
exp1 -> exp2

exp2 -> exp2 ANDAND exp3
exp2 -> exp3

exp3 -> exp3 OR_OP exp4
exp3 -> exp4

exp4 -> exp4 AND_OP exp5
exp4 -> exp5

exp5 -> exp5 EQ exp6
exp5 -> exp5 NOTEQ exp6
exp5 -> exp6

exp6 -> exp6 LT exp7
exp6 -> exp6 GT exp7
exp6 -> exp6 LTEQ exp7
exp6 -> exp6 GTEQ exp7
exp6 -> exp7

exp7 -> exp7 SHL_OP exp8
exp7 -> exp7 SHR_OP exp8
exp7 -> exp8

exp8 -> exp8 PLUS exp9
exp8 -> exp8 MINUS exp9
exp8 -> exp9

exp9 -> exp9 MUL_OP exp10
exp9 -> exp9 DIV_OP exp10
exp9 -> exp10

exp10 -> NOT_OP exp11
exp10 -> MINUS exp11
exp10 -> exp11

exp11 -> ID LSQUARE exp RSQUARE
exp11 -> INT_NUM
exp11 -> ID
exp11 -> LPAR exp RPAR
```

## Building the LR(1) Parsing Table

The LR(1) parsing table is built using the Parser::build() method, which constructs the configuration sets, action table, and goto table. The implementation proceeds as follows:

* Initializes the configuration sets with the initial configuration set and its closure.
* Iterates through the configuration sets and computes transitions for each configuration in the set. Transitions are grouped based on the symbol causing the transition.
* For each transition, the method finds or creates a new configuration set and updates the appropriate action or goto table entry.
* Repeats this process until no changes are made to the configuration sets or tables.
* Considers reduction cases for each complete configuration in the configuration sets, updating the action table accordingly.

## Parsing Using the LR(1) Parsing Table

To parse a series of C tokens, the parser reads the input and consults the LR(1) parsing table to determine the appropriate action to take for each token. The parser follows the steps of shifting, reducing, or accepting based on the action specified in the parsing table. Upon completion, the parser produces the corresponding parse tree for the given input.

## Handling Shift-Reduce Conflicts (Dangling Else)

The implementation handles shift-reduce conflicts, such as those encountered in if statements, by preferring the "shift" action. This choice ensures that the "else" part of an if statement is associated with the nearest "if". In case of a reduce/reduce conflict, the implementation reports an error and terminates the program.

## Miscellaneous
The implementation produces two executable files: "builder" and "parser". The "builder" program constructs the LR(1) parser state, serializes it, and saves it to a file (e.g., "parser.xml") on disk. The "parser" program reads the serialized parser state file, deserializes it, and parses the input using the deserialized parser state, eliminating the need to rebuild the tables.

> Usage:
>  * Builder: builder ./parser.xml (auto invoked by make)
>  * Parser: parser ./parser.xml (auto invoked by run_parser.sh)

## Conclusion
This report presented the design and implementation of an LR(1) parser for a simplified C language. The parser is capable of parsing C tokens and producing parse trees using the LR(1) parsing table. The implementation handles shift-reduce conflicts and provides two executable files, "builder" and "parser", to streamline the process of building and parsing the input.