#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

enum Token
{
    INT,
    MAIN,
    DO,
    ELSE,
    RETURN,
    VOID,
    BREAK,
    IF,
    WHILE,
    READ,
    WRITE,
    INT_NUM,
    ID,
    LBRACE,
    RBRACE,
    LSQUARE,
    RSQUARE,
    LPAR,
    RPAR,
    SEMI,
    PLUS,
    MINUS,
    MUL_OP,
    DIV_OP,
    AND_OP,
    OR_OP,
    NOT_OP,
    ASSIGN,
    LT,
    GT,
    LTEQ,
    GTEQ,
    SHL_OP,
    SHR_OP,
    EQ,
    NOTEQ,
    ANDAND,
    OROR,
    COMMA
};

struct DFAState;
typedef std::shared_ptr<DFAState> DFAStatePtr;


struct DFAState
{
    std::unordered_map<char, DFAStatePtr> transitions;
    bool is_final = false;
    Token token;
};

class DFA
{
private:
    
    std::vector<DFAStatePtr> states;

public:
    DFAStatePtr startState;
    DFAStatePtr currState;
    DFAStatePtr makeNewState();
    bool move(char c);
    bool match(std::string s);
    DFA();
};