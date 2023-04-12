#include "scanner.hpp"
#include <cctype>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

void Scanner::buildINT_NUM()
{
    auto digitNFA = NFA("0123456789").selfRepeat();
    digitNFA.endState->token = INT_NUM;
    nfas.emplace_back(digitNFA);
}

void Scanner::buildID()
{
    auto digitNFA = NFA("0123456789");
    auto letterNFA = NFA("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
    auto letterPlus = letterNFA.selfRepeat();
    auto digitLetterUnderscore = (digitNFA.alternativeWith(letterNFA).alternativeWith(NFA("_"))).starOp();
    auto id = letterPlus.concatWith(digitLetterUnderscore);
    id.endState->token = ID;
    nfas.emplace_back(id);
}

void Scanner::buildReservedWords()
{

    for (auto &p : reservedWords)
    {
        std::string reservedWord = p.first;
        Token token = p.second;
        reservedNfas.push_back({reservedWord, token});
        nfas.push_back({reservedWord, token});
    }
}

void Scanner::buildSpecialSymbols()
{
    for (auto &p : specialSymbols)
    {
        std::string reservedWord = p.first;
        Token token = p.second;
        nfas.push_back({reservedWord, token});
    }
}

bool Scanner::test(std::string component, const std::vector<NFA> &selectedNFAs)
{
    int nextUnmatchedIdx = 0;
    Token latestMatchedToken = UNKNOWN;
    std::vector<Token> tempMatched;
    std::stack<std::tuple<StatePtr, int, std::unordered_set<StatePtr>>> stateStack;

    for (auto &nfa : selectedNFAs)
    {
        stateStack.push({nfa.startState, 0, {}});
    }
    while (nextUnmatchedIdx < int(component.size()))
    {
        nextUnmatchedIdx = 0;
        latestMatchedToken = UNKNOWN;
        while (!stateStack.empty())
        {
            StatePtr currState = std::get<0>(stateStack.top());
            int nextIdx = std::get<1>(stateStack.top());
            std::unordered_set<StatePtr> visited = std::get<2>(stateStack.top());
            stateStack.pop();
            if (currState->token != UNKNOWN &&
                (nextIdx > nextUnmatchedIdx ||
                 (nextIdx == nextUnmatchedIdx && currState->token < latestMatchedToken)))
            {
                nextUnmatchedIdx = nextIdx;
                latestMatchedToken = currState->token;
            }
            if (nextIdx < int(component.size()) && currState->transitions.count(component[nextIdx]))
            {
                for (auto &nextState : currState->transitions[component[nextIdx]])
                {

                    stateStack.push({nextState, nextIdx + 1, {}});
                }
            }
            if (currState->transitions.count('\0'))
            {
                for (auto &nextState : currState->transitions['\0'])
                {
                    if (visited.count(nextState))
                        continue;
                    std::unordered_set<StatePtr> newVisited = visited;
                    newVisited.insert(currState);
                    stateStack.push({nextState, nextIdx, newVisited});
                }
            }
        }
        if (nextUnmatchedIdx == 0)
        {
            return false;
        }
        tempMatched.push_back(latestMatchedToken);
        for (auto &nfa : selectedNFAs)
        {
            stateStack.push({nfa.startState, nextUnmatchedIdx, {}});
        }
    }
    for (auto &t : tempMatched)
        matchedTokens.push_back(t);
    return true;
}

Scanner::Scanner(std::string s)
{
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    components = std::vector<std::string>{begin, end};
}

bool Scanner::compile()
{
    buildReservedWords();
    buildID();
    buildINT_NUM();
    buildSpecialSymbols();
    for (auto &s : components)
    {
        if (!test(s, reservedNfas))
        {
            if (!test(s, nfas))
                return false;
        }
    }
    return true;
}