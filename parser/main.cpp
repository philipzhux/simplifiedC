#include "parser.h"
#include <iostream>

int main()
{
    NonTerminal _sPrime("S'", Symbol::getId());
    NonTerminal _s("S", Symbol::getId());
    NonTerminal _x("X", Symbol::getId());
    Terminal _$("$", $, Symbol::getId());
    Terminal _a("a", A, Symbol::getId());
    Terminal _b("b", B, Symbol::getId());

    Parser parser(_sPrime, {_s}, _$);
    parser.addProduction(_s, {_x, _x});
    parser.addProduction(_x, {_a, _x});
    parser.addProduction(_x, {_b});
    parser.build();
    parser.printProductions();
    parser.printConfigurationSets();
    parser.printActionTable();
    parser.printGotoTable();

    // sample LR(1) grammar from Stanford CS143 lecture 110
    if (!parser.parse({_b, _a, _a, _b}))
    {
        std::cout << "Syntax error" << std::endl;
    }
    else
    {
        std::cout << "Accepted" << std::endl;
    }
}

    // Scanner scanner("test.txt");
    // scanner.compile();
    // convert enum token to terminal symbols
    // Terminal _int("INT", INT, Symbol::getId());
    // Terminal _main("MAIN", MAIN, Symbol::getId());
    // Terminal _do("DO", DO, Symbol::getId());
    // Terminal _else("ELSE", ELSE, Symbol::getId());
    // Terminal _return("RETURN", RETURN, Symbol::getId());
    // Terminal _void("VOID", VOID, Symbol::getId());
    // Terminal _break("BREAK", BREAK, Symbol::getId());
    // Terminal _if("IF", IF, Symbol::getId());
    // Terminal _while("WHILE", WHILE, Symbol::getId());
    // Terminal _read("READ", READ, Symbol::getId());
    // Terminal _write("WRITE", WRITE, Symbol::getId());
    // Terminal _int_num("INT_NUM", INT_NUM, Symbol::getId());
    // Terminal _id("ID", ID, Symbol::getId());
    // Terminal _lbrace("LBRACE", LBRACE, Symbol::getId());
    // Terminal _rbrace("RBRACE", RBRACE, Symbol::getId());
    // Terminal _lsquare("LSQUARE", LSQUARE, Symbol::getId());
    // Terminal _rsquare("RSQUARE", RSQUARE, Symbol::getId());
    // Terminal _lpar("LPAR", LPAR, Symbol::getId());
    // Terminal _rpar("RPAR", RPAR, Symbol::getId());
    // Terminal _semi("SEMI", SEMI, Symbol::getId());
    // Terminal _plus("PLUS", PLUS, Symbol::getId());
    // Terminal _minus("MINUS", MINUS, Symbol::getId());
    // Terminal _mul_op("MUL_OP", MUL_OP, Symbol::getId());
    // Terminal _div_op("DIV_OP", DIV_OP, Symbol::getId());
    // Terminal _and_op("AND_OP", AND_OP, Symbol::getId());
    // Terminal _or_op("OR_OP", OR_OP, Symbol::getId());