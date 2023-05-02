/*
 * Created on Tue Feb 21 2023
 *
 * Copyright (c) 2023 Philip Zhu Chuyan <me@cyzhudev>
 */

#include "code.hpp"
#include "utils.hpp"

ASTGen::Code::Code(): symbolTable(this)
{
    codeLines.push_back("move $fp,$sp"); // initialize $fp to $sp, $fp being start of stack
}

std::vector<std::string> &ASTGen::Code::getAssembly()
{
    return codeLines;
}

void ASTGen::Code::reg2Sym(ASTGen::Register reg, ASTGen::SymEntry symbolIdx)
{
    addAsmLine(::stringFormat("sw %s,%d($fp)", reg.c_str(), int(symbolIdx) * -4));
}

void ASTGen::Code::sym2Reg(ASTGen::SymEntry symbolIdx, ASTGen::Register reg)
{
    addAsmLine(::stringFormat("lw %s,%d($fp)", reg.c_str(), int(symbolIdx) * -4));
}

void ASTGen::Code::getAddressOf(ASTGen::SymEntry symbolIdx, ASTGen::Register dest)
{
    addAsmLine(::stringFormat("add %s,%d,$fp", dest.c_str(), int(symbolIdx) * -4));
}


void ASTGen::Code::sym2Sym(ASTGen::SymEntry destSymIdx, ASTGen::SymEntry srcSymIdx)
{
    Register t1("$t1");
    sym2Reg(srcSymIdx, t1);
    reg2Sym(t1, destSymIdx);
}

void ASTGen::Code::int2Reg(int intLiteral, ASTGen::Register reg)
{
    addAsmLine(::stringFormat("li %s,%d", reg.c_str(), intLiteral));
}

void ASTGen::Code::mem2Reg(ASTGen::SymEntry memAddr, ASTGen::Register reg)
{
    // first move address from memory to $t1
    Register t1("$t1");
    sym2Reg(memAddr, t1);
    // then load value from memory address to register
    addAsmLine(::stringFormat("lw %s,0(%s)", reg.c_str(), t1.c_str()));
}

void ASTGen::Code::reg2Mem(ASTGen::Register reg, ASTGen::SymEntry memAddr)
{
    // first move address from memory to $t1
    Register t1("$t1");
    sym2Reg(memAddr, t1);
    // then store value from register to memory address
    addAsmLine(::stringFormat("sw %s,0(%s)", reg.c_str(), t1.c_str()));
}

void ASTGen::Code::mem2Sym(ASTGen::SymEntry memAddr, ASTGen::SymEntry symbolIdx)
{
    // first move address from memory to $t1
    Register t1("$t1");
    sym2Reg(memAddr, t1);
    Register t2("$t2");
    // then load value from memory address to $t2
    addAsmLine(::stringFormat("lw %s,0(%s)", t2.c_str(), t1.c_str()));
    // then store value from $t2 to the symbol
    reg2Sym(t2, symbolIdx);
}

void ASTGen::Code::addAsmLine(std::string line)
{
    codeLines.push_back(line);
}

void ASTGen::Code::moveStack(int offset)
{
    addAsmLine(::stringFormat("addi $sp,$sp,%d", offset));
}

void ASTGen::Code::sysRead(ASTGen::SymEntry symbolIdx)
{   
    addAsmLine("li $v0,5");
    addAsmLine("syscall");
    reg2Sym(Register("$v0"),symbolIdx);
}

void ASTGen::Code::sysWrite(ASTGen::SymEntry symbolIdx)
{   
    addAsmLine("li $v0,1");
    sym2Reg(symbolIdx, Register("$a0"));
    addAsmLine("syscall");
}

void ASTGen::Code::sysPrintNewLine()
{
    addAsmLine("li $v0,11");
    addAsmLine("li $a0,10");
    addAsmLine("syscall");
}

std::string ASTGen::Code::getLabel(std::string prefix)
{
    if (currentLabelCount.count(prefix) == 0)
    {
        currentLabelCount[prefix] = 0;
    }
    return ::stringFormat("%s%d", prefix.c_str(), currentLabelCount[prefix]++);
}

std::string ASTGen::Code::getLabel()
{
    return getLabel("label");
}