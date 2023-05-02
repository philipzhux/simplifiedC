# Simplified C Compiler Report

This report covers the main aspects of implementing a compiler for a simplified C language. The compiler is built using a scanner and a parser that have been developed in previous projects, and it also includes an AST generator to generate the code.

## 1. Implementing the Scanner (Lexer)

The scanner is based on a previously implemented NFA regex engine, which is responsible for tokenizing the input source code. It converts the input source code into a sequence of tokens that represent the lexical elements of the language, such as keywords, identifiers, literals, and operators.

The scanner uses regular expressions to define the patterns of these lexical elements, and the NFA regex engine matches these patterns in the input source code. The scanner then returns a stream of tokens to the parser, which will be used to build the syntax tree.

## 2. Implementing the Parser

The parser is an LR(1) shift-reduce parser that was developed in a previous project. It takes the stream of tokens produced by the scanner and constructs a syntax tree based on the grammar rules of the simplified C language. The grammar rules are defined using a set of production rules, and the parser uses an LR(1) parsing table to determine the actions (shift, reduce, or accept) to take when parsing the input tokens.

The parser is also responsible for handling syntax errors in the input source code. If a syntax error is detected, the parser will report the error and halt the parsing process.

## 3. Code Generation

The code generation process is the most important aspect of the compiler. It involves the following steps:

1. **AST Generation**: The parser creates an Abstract Syntax Tree (AST) during the parsing process using a class called `ASTGen::SyntaxTreeNode`. Each node in the AST represents a syntax element, such as an expression, statement, or declaration. The tree structure represents the hierarchical structure of the source code.

2. **Semantic Actions**: Semantic actions are attached to the production rules in the form of lambda functions. These functions are executed during the parsing process when the parser encounters a reduce action. The semantic actions are responsible for generating the code as the AST is being built.

For example, the code generation for an while statement can be achieved with the following action lambda:

```cpp
parser.addProduction(
    _while_statement, {_while, _lpar, _exp, _rpar, _code_block},
    [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
    {
        auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::WHILE_STATEMENT);
        newNode->children.push_back(rhsNodes[2]); // condition exp
        newNode->children.push_back(rhsNodes[4]); // code block
        return newNode;
    }
);

```

3. **Handling Array Access and Variable Declaration**: The compiler handles array access and variable declaration, including static arrays. When the parser encounters an array access or a variable declaration, it generates the appropriate code and updates the symbol table accordingly.

For instance, the action lambda for static array access can be implemented as follows:

```cpp
parser
