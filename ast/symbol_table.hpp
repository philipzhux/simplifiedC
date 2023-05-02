/*
 * Created on Tue Feb 21 2023
 *
 * Copyright (c) 2023 Philip Zhu Chuyan <me@cyzhudev>
 */

#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>


namespace ASTGen
{
    class Code;
    typedef uint32_t SymEntry;
    typedef std::string Register; // to be revised as fine-grained register class
    typedef std::unordered_map<std::string, SymEntry> SymbolMap;
    static const SymEntry NORETURN = SymEntry(-1);
    class SymbolTable
    {
    public:
        SymbolTable(Code* code);
        SymEntry getSymbol(std::string);
        SymEntry declareSymbol(std::string);
        SymEntry declareSymbol(std::string id, int size);
        SymEntry declareTempSymbol();
        void freeTempSymbol(SymEntry);
        void printSymbolTable();
        bool isSymbolExist(std::string);
        bool isEntTemp(SymEntry);

    private:
        SymbolMap map;
        std::unordered_set<SymEntry> tempSet;
        std::unordered_set<SymEntry> freedTempSyms;
        SymEntry currIdx;
        Code* code;
    };
};
