# Simplified C Compiler Report

This report covers the main aspects of implementing a compiler for a simplified C language. The compiler is built using a scanner and a parser that have been developed in previous projects, and it also includes an AST generator to generate the code.

## 1. Implementing the Scanner (Lexer)

The scanner is based on a previously implemented NFA regex engine, which is responsible for tokenizing the input source code. It converts the input source code into a sequence of tokens that represent the lexical elements of the language, such as keywords, identifiers, literals, and operators.

The scanner uses regular expressions to define the patterns of these lexical elements, and the NFA regex engine matches these patterns in the input source code. The scanner then returns a stream of tokens to the parser, which will be used to build the syntax tree.

## 2. Implementing the Parser

The parser is an LR(1) shift-reduce parser that was developed in a previous project. It takes the stream of tokens produced by the scanner and constructs a syntax tree based on the grammar rules of the simplified C language. The grammar rules are defined using a set of production rules, and the parser uses an LR(1) parsing table to determine the actions (shift, reduce, or accept) to take when parsing the input tokens.

The parser is also responsible for handling syntax errors in the input source code. If a syntax error is detected, the parser will report the error and halt the parsing process.

## 3. Code Generation

The code generation process is the most important aspect of the compiler. I reused the code from project 1 and adapt it to integrate with my previously implemented lexer and parser. Similar to project 1, I adopts an one-pass approach only and due to time constraint I did not implement optimization for register allocation (a graph coloring algo could be used in this case, but it requires 3AC code to be generated instead of compiling in one pass). My implementation involves the following steps:

1. **AST Generation**: The parser creates an Abstract Syntax Tree (AST) during the parsing process using a class called `ASTGen::SyntaxTreeNode`. Each node in the AST represents a syntax element, such as an expression, statement, or declaration. The tree structure represents the hierarchical structure of the source code.

2. **Semantic Actions**: Semantic actions are attached to the production rules in the form of lambda functions. These functions are executed during the parsing process when the parser encounters a reduce action. The semantic actions are responsible for generating the code as the AST is being built.

For example, the code generation for an while statement can be achieved with the following action lambda (**very similar approach to bison**):

```cpp
parser.addProduction(
    _while_statement, {_while, _lpar, _exp, _rpar, _code_block},
    [&](std::vector< std::shared_ptr<ASTGen::SyntaxTreeNode>>
    rhsNodes) 
    -> std::shared_ptr<ASTGen::SyntaxTreeNode>
    {
        auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>
        (ASTGen::WHILE_STATEMENT);
        // condition exp
        newNode->children.push_back(rhsNodes[2]); 
        // code block
        newNode->children.push_back(rhsNodes[4]);
        return newNode;
    }
);

```

Under the hood, the codeGen part nests in `newNode->generateCode()`, taking ASSIGN_STATEMENT as an example:
```cpp
...
case ASSIGN_STATEMENT:
{
SymEntry rightSym = children[1]->generateCode(code);
if(children[1]->nodeType == ARRAY_ACCESS) 
   code.mem2Sym(rightSym, rightSym); //dereferencing
SymEntry leftSym = children[0]->generateCode(code);
if(children[0]->nodeType == ARRAY_ACCESS)
{
   // store value in rightSym to the address that leftSym points to
   code.sym2Reg(leftSym, reg3); // reg3 is the address
   code.sym2Reg(rightSym, reg4); // reg4 is the value
   code.addAsmLine(::stringFormat("%s %s,0(%s)"
   , "sw", reg4.c_str(), reg3.c_str()));
}
else
{
   code.sym2Sym(leftSym, rightSym); // code::sym2Sym(dest,src)
}

if (code.symbolTable.isEntTemp(leftSym))
   code.symbolTable.freeTempSymbol(leftSym);
if (code.symbolTable.isEntTemp(rightSym))
   code.symbolTable.freeTempSymbol(rightSym);
return ASTGen::NORETURN;
...
```

3. **Handling Array Access and Variable Declaration**: The compiler handles array access and variable declaration, including static arrays. When the parser encounters an array access or a variable declaration, it generates the appropriate code and updates the symbol table accordingly.


4. **Control Statements**: The compiler supports control statements such as `while`, `do-while`, and `if-else`. The parser generates the appropriate code for these constructs during the parsing process. The code generation for control statements involves creating and managing labels, generating code for conditions and branches, and ensuring that the control flow is correctly implemented.

5. **Function Call and Return Statement**: The compiler also supports return statements. The `return` statement is used to terminate the program. The code generation for the return statement involves generating a jump instruction to the end of the program.



The steps mentioned above outline the main components of the code generation process in the simplified C compiler. By utilizing the scanner, parser, and ASTGen classes, the compiler can effectively generate MIPS code from the input source code, taking into account different language constructs such as variable declarations, array access, control statements, and function calls.