#include "grammar.h"
#include <stdexcept>

void parserWrapper::buildGrammar()
{
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

    // mapping between token type and terminal symbol
    tokenToTerminal = {
        {INT, _int},
        {SEMI, _semi},
        {COMMA, _comma},
        {ASSIGN, _assign},
        {ID, _id},
        {LSQUARE, _lsquare},
        {RSQUARE, _rsquare},
        {LBRACE, _lbrace},
        {RBRACE, _rbrace},
        {IF, _if},
        {ELSE, _else},
        {WHILE, _while},
        {DO, _do},
        {RETURN, _return},
        {READ, _read},
        {WRITE, _write},
        {OROR, _oror},
        {ANDAND, _andand},
        {OR_OP, _or_op},
        {AND_OP, _and_op},
        {EQ, _eq},
        {NOTEQ, _noteq},
        {LT, _lt},
        {GT, _gt},
        {LTEQ, _lteq},
        {GTEQ, _gteq},
        {SHL_OP, _shl_op},
        {SHR_OP, _shr_op},
        {PLUS, _plus},
        {MINUS, _minus},
        {MUL_OP, _mul_op},
        {DIV_OP, _div_op},
        {NOT_OP, _not_op},
        {INT_NUM, _int_num},
        {LPAR, _lpar},
        {RPAR, _rpar}};

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
    NonTerminal _exp10("exp10", Symbol::getId());
    NonTerminal _exp11("exp11", Symbol::getId());

    
    // initialize parser with augmented grammar
    parser = Parser(_sPrime, {_program}, _$);

    // add production rules according to the grammars

    // variable declarations
    parser.addProduction(_program, {_var_declarations, _statements},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            auto mainFunction = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::FUNCTION);
                            for (auto &node : rhsNodes)
                            {
                                if(node==nullptr)
                                    continue;

                                mainFunction->addChild(node);
                            }
                            mainFunction->generateCode(code);
                            return nullptr;
                         });
    parser.addProduction(_var_declarations, {_var_declarations, _var_declaration},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            if (rhsNodes[0] == nullptr)
                            {
                                auto newNote = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::COMPOSITE);
                                newNote->addChild(rhsNodes[1]);
                                return newNote;
                            }
                            rhsNodes[0]->addChild(rhsNodes[1]);
                            return rhsNodes[0];
                         });
    parser.addProduction(_var_declarations, {},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::COMPOSITE);
                         });

    // variable declaration
    parser.addProduction(_var_declaration, {_int, _declaration_list, _semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[1];
                         });

    // declaration list
    parser.addProduction(_declaration_list, {_declaration_list, _comma, _declaration},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {

                            rhsNodes[0]->addChild(rhsNodes[2]);
                            return rhsNodes[0];
                         });
    parser.addProduction(_declaration_list, {_declaration},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::COMPOSITE);
                            newNode->addChild(rhsNodes[0]);
                            return newNode;
                         });

    // declaration
    parser.addProduction(_declaration, {_id, _assign, _int_num},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            // auto sym = code.symbolTable.declareSymbol(rhsNodes[0]->id);
                            // code.int2Reg(rhsNodes[2]->intVal, ASTGen::Register("$t1"));
                            // code.reg2Sym(ASTGen::Register("$t1"), sym);
                            // return nullptr;
                            auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::VAR_DECL_INIT, rhsNodes[0]->id);
                            newNode->intVal = rhsNodes[2]->intVal;
                            return newNode;

                         });
    parser.addProduction(_declaration, {_id, _lsquare, _int_num, _rsquare},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            //  code.symbolTable.declareSymbol(rhsNodes[0]->id, rhsNodes[2]->intVal * 4);
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ARRAY_DECL, rhsNodes[0]->id);
                             newNode->intVal = rhsNodes[2]->intVal;
                             return newNode;
                         });
    parser.addProduction(_declaration, {_id},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                            //  code.symbolTable.declareSymbol(rhsNodes[0]->id);
                            //  return nullptr;
                            return std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::VAR_DECL, rhsNodes[0]->id);
                         });

    // code block
    parser.addProduction(_code_block, {_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_code_block, {_lbrace, _statements, _rbrace},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[1];
                         });

    // statements
    parser.addProduction(_statements, {_statements, _statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             rhsNodes[0]->addChild(rhsNodes[1]);
                             return rhsNodes[0];
                         });
    parser.addProduction(_statements, {_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::COMPOSITE);
                             newNode->addChild(rhsNodes[0]);
                             return newNode;
                         });

    parser.addProduction(_statement, {_assign_statement, _semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_statement, {_control_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_statement, {_read_write_statement, _semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_statement, {_semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });

    // control statement
    parser.addProduction(_control_statement, {_if_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_control_statement, {_while_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_control_statement, {_do_while_statement, _semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_control_statement, {_return_statement, _semi},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });

    // read write statement
    parser.addProduction(_read_write_statement, {_read_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_read_write_statement, {_write_statement},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });

    // assign statement
    parser.addProduction(_assign_statement, {_id, _lsquare, _exp, _rsquare, _assign, _exp},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto arrayAccessNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ARRAY_ACCESS);
                             arrayAccessNode->children.push_back(rhsNodes[0]);
                             arrayAccessNode->children.push_back(rhsNodes[2]);
                             auto assignNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ASSIGN_STATEMENT);
                             assignNode->children.push_back(arrayAccessNode);
                             assignNode->children.push_back(rhsNodes[5]);
                             return assignNode;
                         });
    parser.addProduction(_assign_statement, {_id, _assign, _exp},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto node = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ASSIGN_STATEMENT);
                             node->children.push_back(rhsNodes[0]);
                             node->children.push_back(rhsNodes[2]);
                             return node;
                         });

    // if statement
    parser.addProduction(_if_statement, {_if_stmt},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_if_statement, {_if_stmt, _else, _code_block},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             // code according to syntax_tree.cpp now
                             rhsNodes[0]->children.push_back(rhsNodes[2]); // add else code block to if statement node
                             return rhsNodes[0];
                         });
    // if_stmt
    parser.addProduction(_if_stmt, {_if, _lpar, _exp, _rpar, _code_block},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::IF_STATEMENT);
                             newNode->children.push_back(rhsNodes[2]); // condition exp
                             newNode->children.push_back(rhsNodes[4]); // code block
                             return newNode;
                         });
    parser.addProduction(_while_statement, {_while, _lpar, _exp, _rpar, _code_block},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::WHILE_STATEMENT);
                             newNode->children.push_back(rhsNodes[2]); // condition exp
                             newNode->children.push_back(rhsNodes[4]); // code block
                             return newNode;
                         });
    // do while statement
    parser.addProduction(_do_while_statement, {_do, _code_block, _while, _lpar, _exp, _rpar},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::DO_WHILE_STATEMENT);
                             newNode->children.push_back(rhsNodes[1]); // code block
                             newNode->children.push_back(rhsNodes[4]); // condition exp
                             return newNode;
                         });
    // return statement
    parser.addProduction(_return_statement, {_return},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::RETURN_STATEMENT);
                             return newNode;
                         });
    // read statement
    parser.addProduction(_read_statement, {_read, _lpar, _id, _rpar},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::READ_STATEMENT);
                             newNode->children.push_back(rhsNodes[2]);
                             return newNode;
                         });
    // write statement
    parser.addProduction(_write_statement, {_write, _lpar, _exp, _rpar},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::WRITE_STATEMENT);
                             newNode->children.push_back(rhsNodes[2]);
                             return newNode;
                         });

    // expressions (precedence climbing)
    parser.addProduction(_exp, {_exp1},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp1, {_exp1, _oror, _exp2},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "oror");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp1, {_exp2},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp2, {_exp2, _andand, _exp3},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "andand");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp2, {_exp3},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp3, {_exp3, _or_op, _exp4},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "or");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp3, {_exp4},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp4, {_exp4, _and_op, _exp5},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "and");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp4, {_exp5},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp5, {_exp5, _eq, _exp6},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "eq");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp5, {_exp5, _noteq, _exp6},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "xor");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp5, {_exp6},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp6, {_exp6, _lt, _exp7},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "lt");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp6, {_exp6, _gt, _exp7},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "gt");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp6, {_exp6, _lteq, _exp7},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "lte");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp6, {_exp6, _gteq, _exp7},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "gte");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp6, {_exp7},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp7, {_exp7, _shl_op, _exp8},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "sllv");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp7, {_exp7, _shr_op, _exp8},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "srav");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp7, {_exp8},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp8, {_exp8, _plus, _exp9},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "add");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp8, {_exp8, _minus, _exp9},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "sub");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp8, {_exp9},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp9, {_exp9, _mul_op, _exp10},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "mul");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp9, {_exp9, _div_op, _exp10},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::EXPRESSION, "div");
                             newNode->addChild(rhsNodes[0]);
                             newNode->addChild(rhsNodes[2]);
                             return newNode;
                         });
    parser.addProduction(_exp9, {_exp10},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp10, {_not_op, _exp11},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::UNARY_EXPRESSION, "not");
                             newNode->addChild(rhsNodes[1]);
                             return newNode;
                         });
    parser.addProduction(_exp10, {_minus, _exp11},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             // neg is pseudo-instruction for unary subtraction
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::UNARY_EXPRESSION, "neg");
                             newNode->addChild(rhsNodes[1]);
                             return newNode;
                         });
    parser.addProduction(_exp10, {_exp11},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp11, {_id, _lsquare, _exp, _rsquare},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             auto newNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ARRAY_ACCESS);
                             newNode->addChild(rhsNodes[0]); // array name
                             newNode->addChild(rhsNodes[2]); // index
                             return newNode;
                         });
    parser.addProduction(_exp11, {_int_num},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp11, {_id},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[0];
                         });
    parser.addProduction(_exp11, {_lpar, _exp, _rpar},
                         [&](std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>> rhsNodes) -> std::shared_ptr<ASTGen::SyntaxTreeNode>
                         {
                             return rhsNodes[1];
                         });
    parser.build();
    // return parser;

    // std::ofstream file(path);
    // cereal::XMLOutputArchive archive(file);
    // archive(CEREAL_NVP(parser), CEREAL_NVP(tokenToTerminal));
    // return parser.configurationSets.size();
}

parserWrapper::parserWrapper(std::string parserStatePath)
{
    buildGrammar();
    // std::ifstream is(parserStatePath);
    // if (!is.good())
    //     throw std::runtime_error("Cannot open parser state file: " + parserStatePath);
    // cereal::XMLInputArchive archive(is);
    // archive(parser, tokenToTerminal);
}
