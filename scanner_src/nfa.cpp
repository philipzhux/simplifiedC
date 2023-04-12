#include "nfa.hpp"
#include <stack>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

StatePtr NFA::makeNewState()
{
    return std::make_shared<NFAState>();
}

NFA::NFA()
{
    startState = NFA::makeNewState();
    currState = startState;
    endState = startState;
}

NFA::NFA(std::string reservedWord, Token token): NFA()
{
    StatePtr prev = startState;
    for (size_t i = 0; i < reservedWord.size(); i++)
        {

            char c = reservedWord[i];
            StatePtr curr = NFA::makeNewState();
            NFA::addTransition(prev,curr,c);
            prev = curr;
            if (i == reservedWord.size() - 1)
            {
                endState = curr;
                curr->token = token;
            }
        }

}

NFA::NFA(std::string anonymousChars): NFA()
{  
    endState = NFA::makeNewState();
    for (size_t i = 0; i < anonymousChars.size(); i++)
    {
        char c = anonymousChars[i];
        auto newState = NFA::makeNewState();
        NFA::addTransition(startState,newState,c);
        NFA::addTransition(newState,endState,'\0');
    }
}

void NFA::addTransition(const StatePtr& srcState, const StatePtr& destState, char c)
{

    if(srcState->transitions.count(c))
            {
                srcState->transitions[c].push_back(destState);
            }
            else
            {
                srcState->transitions[c] = std::vector<StatePtr>{destState};
            }

}


NFA NFA::concatWith(const NFA& others)
{
    NFA retNFA = *this;
    NFA::addTransition(retNFA.endState,others.startState,'\0');
    retNFA.endState->token = UNKNOWN;
    retNFA.endState = others.endState;
    return retNFA;
}

NFA NFA::alternativeWith(const NFA& others)
{
    NFA retNFA = *this;
    auto newStart = NFA::makeNewState();
    auto newEnd = NFA::makeNewState();
    NFA::addTransition(newStart,retNFA.startState,'\0');
    NFA::addTransition(newStart,others.startState,'\0');
    NFA::addTransition(retNFA.endState,newEnd,'\0');
    NFA::addTransition(others.endState,newEnd,'\0');
    retNFA.endState->token = UNKNOWN;
    retNFA.startState = newStart;
    retNFA.endState = newEnd;
    retNFA.currState =  retNFA.startState;
    return retNFA;
}


NFA NFA::selfRepeat()
{
    NFA retNFA = *this;
    NFA::addTransition(retNFA.endState,retNFA.startState,'\0');
    return retNFA;
}

NFA NFA::starOp()
{
    NFA retNFA = selfRepeat();
    NFA::addTransition(retNFA.startState,retNFA.endState,'\0');
    return retNFA;
}



namespace TokenMapper
{
    std::unordered_map<Token, std::string> token_to_string = {
        {INT, "INT"},
        {MAIN, "MAIN"},
        {DO, "DO"},
        {ELSE, "ELSE"},
        {RETURN, "RETURN"},
        {VOID, "VOID"},
        {BREAK, "BREAK"},
        {IF, "IF"},
        {WHILE, "WHILE"},
        {READ, "READ"},
        {WRITE, "WRITE"},
        {INT_NUM, "INT_NUM"},
        {ID, "ID"},
        {LBRACE, "LBRACE"},
        {RBRACE, "RBRACE"},
        {LSQUARE, "LSQUARE"},
        {RSQUARE, "RSQUARE"},
        {LPAR, "LPAR"},
        {RPAR, "RPAR"},
        {SEMI, "SEMI"},
        {PLUS, "PLUS"},
        {MINUS, "MINUS"},
        {MUL_OP, "MUL_OP"},
        {DIV_OP, "DIV_OP"},
        {AND_OP, "AND_OP"},
        {OR_OP, "OR_OP"},
        {NOT_OP, "NOT_OP"},
        {ASSIGN, "ASSIGN"},
        {LT, "LT"},
        {GT, "GT"},
        {LTEQ, "LTEQ"},
        {GTEQ, "GTEQ"},
        {SHL_OP, "SHL_OP"},
        {SHR_OP, "SHR_OP"},
        {EQ, "EQ"},
        {NOTEQ, "NOTEQ"},
        {ANDAND, "ANDAND"},
        {OROR, "OROR"},
        {COMMA, "COMMA"},
        {UNKNOWN, "UNKNOWN"}
    };
}


bool NFA::isReservedToken(Token t)
{
    return t<=WRITE;
}