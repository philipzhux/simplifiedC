/*
 * Created on Tue Feb 21 2023
 *
 * Copyright (c) 2023 Philip Zhu Chuyan <me@cyzhudev>
 */

#include "symbol_table.hpp"
#include "code.hpp"
ASTGen::SymbolTable::SymbolTable(ASTGen::Code* _code)
{
    currIdx = 0;
    code = _code;
}

ASTGen::SymEntry ASTGen::SymbolTable::getSymbol(std::string symbol)
{
    return map[symbol];
}

ASTGen::SymEntry ASTGen::SymbolTable::declareSymbol(std::string symbol)
{
    map[symbol] = ++currIdx;
    code->moveStack(-4);
    return map[symbol];
}

ASTGen::SymEntry ASTGen::SymbolTable::declareSymbol(std::string symbol, int size)
{
    if(size%4!=0)
    {
        size = size/4*4+4;
    }
    currIdx += size/4;
    map[symbol] = currIdx;
    code->moveStack(-size);
    return map[symbol];
}



void ASTGen::SymbolTable::freeTempSymbol(ASTGen::SymEntry symbolIdx)
{
    freedTempSyms.insert(symbolIdx);
}

ASTGen::SymEntry ASTGen::SymbolTable::declareTempSymbol()
{
    SymEntry ret;
    if (freedTempSyms.empty())
    {
        ret = ++currIdx;
        code->moveStack(-4);
        tempSet.insert(ret);
    }
    else
    {
        ret = *freedTempSyms.begin();
        freedTempSyms.erase(ret);
    }

    return ret;
}

bool ASTGen::SymbolTable::isSymbolExist(std::string symbol)
{
    return map.count(symbol) > 0;
}

bool ASTGen::SymbolTable::isEntTemp(ASTGen::SymEntry ent)
{
    return tempSet.count(ent) > 0;
}

// void ASTGen::SymbolTable::printSymbolTable()
// {
//     for (auto &it : map)
//     {

//         printf("%s\t|\t", it.first.c_str(), );
//         if (it.second.inReg)
//         {
//             printf("in_stack\t|\t%s\n",it.second.reg);
//         }
//         else
//         {
//             printf("in_memory\t|\t");
//         }

//     }
// }