#pragma once
// #include "NFAState.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <stack>

struct NFAState;
typedef std::shared_ptr<NFAState> StatePtr;

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
    COMMA,
    UNKNOWN
};

struct NFAState
{
    // the transitions of the state, a map of <symbol, a stack of statePtrs>
    std::unordered_map<char, std::vector<StatePtr>> transitions;
    Token token = UNKNOWN;
};

class NFA
{
    public:
    NFA();
    NFA(std::string reservedWord, Token token);
    NFA(std::string anonymousChars);
    // the start state of the NFA
    StatePtr startState;
    // curruent state of the NFA
    StatePtr currState;
    StatePtr endState;
    // create a new NFAState
    static StatePtr makeNewState();
    NFA selfRepeat();
    NFA concatWith(const NFA&);
    NFA alternativeWith(const NFA&);
    NFA starOp();
    static void addTransition(const StatePtr& srcState, const StatePtr& destState, char c);
    static bool isReservedToken(Token t);
};


namespace TokenMapper
{
    extern std::unordered_map<Token, std::string> token_to_string;
};