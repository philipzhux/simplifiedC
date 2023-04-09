#include "dfa.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

DFAStatePtr DFA::makeNewState()
{
    DFAStatePtr newState = std::make_shared<DFAState>();
    states.push_back(newState);
    return newState;
}

bool DFA::match(std::string s)
{
    currState = startState;
    for (char c : s)
    {
        if (currState->transitions.count(c) == 0)
            return false;
        currState = currState->transitions[c];
    }
    return currState->is_final;
}

DFA::DFA()
{
    startState = makeNewState();
    currState = startState;
}


bool DFA::move(char c)
{
    if(currState->transitions.count(c)) {
        currState = currState->transitions[c];
        return true;
    }
    return false;
}
