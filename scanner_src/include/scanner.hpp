#pragma once
#include "nfa.hpp"

class Scanner
{
    public:
    Scanner(std::string);
    std::vector<Token> matchedTokens;
    bool compile();
    private:
    bool test(std::string, const std::vector<NFA>& selectedNFAs);
    std::vector<std::string> components;
    const std::pair<std::string, Token> reservedWords[11] = 
    {
        {"int", INT},
        {"main", MAIN},
        {"do", DO},
        {"else", ELSE},
        {"return", RETURN},
        {"void", VOID},
        {"break", BREAK},
        {"if", IF},
        {"while", WHILE},
        {"scanf", READ},
        {"printf", WRITE},
    };
    const std::pair<std::string, Token> specialSymbols[26] =
    {
        {"{", LBRACE},
        {"}", RBRACE},
        {"[", LSQUARE},
        {"]", RSQUARE},
        {"(", LPAR},
        {")", RPAR},
        {";", SEMI},
        {"+", PLUS},
        {"-", MINUS},
        {"*", MUL_OP},
        {"/", DIV_OP},
        {"&", AND_OP},
        {"|", OR_OP},
        {"!", NOT_OP},
        {"=", ASSIGN},
        {"<", LT},
        {">", GT},
        {"<<", SHL_OP},
        {">>", SHR_OP},
        {"==", EQ},
        {"!=", NOTEQ},
        {"<=", LTEQ},
        {">=", GTEQ},
        {"&&", ANDAND},
        {"||", OROR},
        {",", COMMA}
    };
    std::vector<NFA> reservedNfas;
    std::vector<NFA> nfas;
    void buildReservedWords();
    void buildINT_NUM();
    void buildID();
    void buildSpecialSymbols();

};