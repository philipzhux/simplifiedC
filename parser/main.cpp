#include "parser.h"
#include <iostream>

int main()
{
    // NonTerminal _sPrime("S'", Symbol::getId());
    // NonTerminal _s("S", Symbol::getId());
    // NonTerminal _x("X", Symbol::getId());
    // Terminal _$("$", $, Symbol::getId());
    // Terminal _a("a", A, Symbol::getId());
    // Terminal _b("b", B, Symbol::getId());

    // Parser parser(_sPrime, {_s}, _$);
    // parser.addProduction(_s, {_x, _x});
    // parser.addProduction(_x, {_a, _x});
    // parser.addProduction(_x, {_b});
    // parser.build();
    // parser.printProductions();
    // parser.printConfigurationSets();
    // parser.printActionTable();
    // parser.printGotoTable();

    // // sample LR(1) grammar from Stanford CS143 lecture 110
    // if (!parser.parse({_b, _a, _a, _b}))
    // {
    //     std::cout << "Syntax error" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Accepted" << std::endl;
    // }

    // declare terminal symbols
    Terminal _$("$", $, Symbol::getId());
    Terminal _int("INT", INT, Symbol::getId());
    Terminal _semi("SEMI", SEMI, Symbol::getId());
    Terminal _comma("COMMA", COMMA, Symbol::getId());
    Terminal _assign("ASSIGN", ASSIGN, Symbol::getId());
    Terminal _id("ID", ID, Symbol::getId());
    Terminal _lsquare("LSQUARE", LSQUARE, Symbol::getId());
    Terminal _rsquare("RSQUARE", RSQUARE, Symbol::getId());
    Terminal _lbrace("LBRACE", LBRACE, Symbol::getId());
    Terminal _rbrace("RBRACE", RBRACE, Symbol::getId());
    Terminal _if("IF", IF, Symbol::getId());
    Terminal _else("ELSE", ELSE, Symbol::getId());
    Terminal _while("WHILE", WHILE, Symbol::getId());
    Terminal _do("DO", DO, Symbol::getId());
    Terminal _return("RETURN", RETURN, Symbol::getId());
    Terminal _read("READ", READ, Symbol::getId());
    Terminal _write("WRITE", WRITE, Symbol::getId());
    Terminal _oror("OROR", OROR, Symbol::getId());
    Terminal _andand("ANDAND", ANDAND, Symbol::getId());
    Terminal _or_op("OR_OP", OR_OP, Symbol::getId());
    Terminal _and_op("AND_OP", AND_OP, Symbol::getId());
    Terminal _eq("EQ", EQ, Symbol::getId());
    Terminal _noteq("NOTEQ", NOTEQ, Symbol::getId());
    Terminal _lt("LT", LT, Symbol::getId());
    Terminal _gt("GT", GT, Symbol::getId());
    Terminal _lteq("LTEQ", LTEQ, Symbol::getId());
    Terminal _gteq("GTEQ", GTEQ, Symbol::getId());
    Terminal _shl_op("SHL_OP", SHL_OP, Symbol::getId());
    Terminal _shr_op("SHR_OP", SHR_OP, Symbol::getId());
    Terminal _plus("PLUS", PLUS, Symbol::getId());
    Terminal _minus("MINUS", MINUS, Symbol::getId());
    Terminal _mul_op("MUL_OP", MUL_OP, Symbol::getId());
    Terminal _div_op("DIV_OP", DIV_OP, Symbol::getId());
    Terminal _not_op("NOT_OP", NOT_OP, Symbol::getId());
    Terminal _int_num("INT_NUM", INT_NUM, Symbol::getId());
    Terminal _lpar("LPAR", LPAR, Symbol::getId());
    Terminal _rpar("RPAR", RPAR, Symbol::getId());

    // declare non-terminal symbols
    NonTerminal _sPrime("S'", Symbol::getId());
    NonTerminal _program("program", Symbol::getId());
    NonTerminal _var_declarations("var_declarations", Symbol::getId());
    NonTerminal _var_declaration("var_declaration", Symbol::getId());
    NonTerminal _declaration_list("declaration_list", Symbol::getId());
    NonTerminal _declaration("declaration", Symbol::getId());
    NonTerminal _code_block("code_block", Symbol::getId());
    NonTerminal _statements("statements", Symbol::getId());
    NonTerminal _statement("statement", Symbol::getId());
    NonTerminal _assign_statement("assign_statement", Symbol::getId());
    NonTerminal _control_statement("control_statement", Symbol::getId());
    NonTerminal _read_write_statement("read_write_statement", Symbol::getId());
    NonTerminal _if_statement("if_statement", Symbol::getId());
    NonTerminal _if_stmt("if_stmt", Symbol::getId());
    NonTerminal _while_statement("while_statement", Symbol::getId());
    NonTerminal _do_while_statement("do_while_statement", Symbol::getId());
    NonTerminal _return_statement("return_statement", Symbol::getId());
    NonTerminal _read_statement("read_statement", Symbol::getId());
    NonTerminal _write_statement("write_statement", Symbol::getId());
    NonTerminal _exp("exp", Symbol::getId());
    NonTerminal _exp1("exp1", Symbol::getId());
    NonTerminal _exp2("exp2", Symbol::getId());
    NonTerminal _exp3("exp3", Symbol::getId());
    NonTerminal _exp4("exp4", Symbol::getId());
    NonTerminal _exp5("exp5", Symbol::getId());
    NonTerminal _exp6("exp6", Symbol::getId());
    NonTerminal _exp7("exp7", Symbol::getId());
    NonTerminal _exp8("exp8", Symbol::getId());
    NonTerminal _exp9("exp9", Symbol::getId());

    // initialize parser
    Parser parser(_sPrime, {_program}, _$);

    // add production rules according to the grammars
    parser.addProduction(_program, {_var_declarations, _statements});
    parser.addProduction(_var_declarations, {_var_declarations, _var_declaration});
    parser.addProduction(_var_declarations, {_var_declaration});
    parser.addProduction(_var_declarations, {});
    parser.addProduction(_var_declaration, {_int, _declaration_list, _semi});
    parser.addProduction(_declaration_list, {_declaration_list, _comma, _declaration});
    parser.addProduction(_declaration_list, {_declaration});
    parser.addProduction(_declaration, {_id, _assign, _int_num});
    parser.addProduction(_declaration, {_id, _lsquare, _int_num, _rsquare});
    parser.addProduction(_declaration, {_id});
    parser.addProduction(_code_block, {_statement});
    parser.addProduction(_code_block, {_lbrace, _statements, _rbrace});
    parser.addProduction(_statements, {_statements, _statement});
    parser.addProduction(_statements, {_statement});
    parser.addProduction(_statement, {_assign_statement, _semi});
    parser.addProduction(_statement, {_control_statement});
    parser.addProduction(_statement, {_read_write_statement, _semi});
    parser.addProduction(_statement, {_semi});
    parser.addProduction(_control_statement, {_if_statement});
    parser.addProduction(_control_statement, {_while_statement});
    parser.addProduction(_control_statement, {_do_while_statement, _semi});
    parser.addProduction(_control_statement, {_return_statement, _semi});
    parser.addProduction(_read_write_statement, {_read_statement});
    parser.addProduction(_read_write_statement, {_write_statement});
    parser.addProduction(_assign_statement, {_id, _lsquare, _exp, _rsquare, _assign, _exp});
    parser.addProduction(_assign_statement, {_id, _assign, _exp});
    parser.addProduction(_if_statement, {_if_stmt});
    parser.addProduction(_if_statement, {_if_stmt, _else, _code_block});
    parser.addProduction(_if_stmt, {_if, _lpar, _exp, _rpar, _code_block});
    parser.addProduction(_while_statement, {_while, _lpar, _exp, _rpar, _code_block});
    parser.addProduction(_do_while_statement, {_do, _code_block, _while, _lpar, _exp, _rpar});
    parser.addProduction(_return_statement, {_return});
    parser.addProduction(_read_statement, {_read, _lpar, _id, _rpar});
    parser.addProduction(_write_statement, {_write, _lpar, _exp, _rpar});
    parser.addProduction(_exp, {_exp, _oror, _exp1});
    parser.addProduction(_exp, {_exp1});
    parser.addProduction(_exp1, {_exp1, _andand, _exp2});
    parser.addProduction(_exp1, {_exp2});
    parser.addProduction(_exp2, {_exp2, _or_op, _exp3});
    parser.addProduction(_exp2, {_exp3});
    parser.addProduction(_exp3, {_exp3, _and_op, _exp4});
    parser.addProduction(_exp3, {_exp4});
    parser.addProduction(_exp4, {_exp5, _eq, _exp6});
    parser.addProduction(_exp4, {_exp5, _noteq, _exp6});
    parser.addProduction(_exp4, {_exp5});
    parser.addProduction(_exp5, {_exp6, _lt, _exp7});
    parser.addProduction(_exp5, {_exp6, _gt, _exp7});
    parser.addProduction(_exp5, {_exp6, _lteq, _exp7});
    parser.addProduction(_exp5, {_exp6, _gteq, _exp7});
    parser.addProduction(_exp5, {_exp6, _shl_op, _exp7});
    parser.addProduction(_exp5, {_exp6, _shr_op, _exp7});
    parser.addProduction(_exp5, {_exp6});
    parser.addProduction(_exp6, {_exp6, _plus, _exp7});
    parser.addProduction(_exp6, {_exp6, _minus, _exp7});
    parser.addProduction(_exp6, {_exp7});
    parser.addProduction(_exp7, {_exp7, _mul_op, _exp8});
    parser.addProduction(_exp7, {_exp7, _div_op, _exp8});
    parser.addProduction(_exp7, {_exp8});
    parser.addProduction(_exp8, {_not_op, _exp9});
    parser.addProduction(_exp8, {_minus, _exp9});
    parser.addProduction(_exp8, {_exp9});
    parser.addProduction(_exp9, {_id, _lsquare, _exp, _rsquare});
    parser.addProduction(_exp9, {_int_num});
    parser.addProduction(_exp9, {_id});
    parser.addProduction(_exp9, {_lpar, _exp, _rpar});
    parser.build();
    std::cout<<"Rep count: "<<parser.repCount<<std::endl;    std::cout<< "parser built."<< std::endl;

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