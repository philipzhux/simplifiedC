/*
 * Created on Tue Feb 21 2023
 *
 * Copyright (c) 2023 Philip Zhu Chuyan <me@cyzhudev>
 */

#include <assert.h>
#include "syntax_tree.hpp"
#include "code.hpp"
#include "utils.hpp"

ASTGen::SyntaxTreeNode::SyntaxTreeNode(ASTGen::NodeType _nodeType) : nodeType(_nodeType) {}

ASTGen::SyntaxTreeNode::SyntaxTreeNode(ASTGen::NodeType _nodeType, std::string stringVal) : nodeType(_nodeType)
{
    switch (nodeType)
    {
    case ID:
        id = stringVal;
        break;
    case EXPRESSION:
        op = stringVal;
        break;
    case UNARY_EXPRESSION:
        op = stringVal;
        break;
    default:
        break;
    }
}
ASTGen::SyntaxTreeNode::SyntaxTreeNode(ASTGen::NodeType _nodeType, int _intVal) : nodeType(_nodeType)
{
    intVal = _intVal;
}

ASTGen::SymEntry ASTGen::SyntaxTreeNode::generateCode(ASTGen::Code &code)
{
    Register reg1("$t1"), reg2("$t2"), reg3("$t3"), reg4("t4");
    switch (nodeType)
    {
    case FUNCTION:
    {
        // add label for function, in this case only main
        std::string functionLabel = "main";
        std::string functionEnd = "mainEnd";
        code.addAsmLine(functionLabel + ":");
        for (auto &child : children)
        {
            if (child == nullptr)
                continue;
            child->generateCode(code);
        }
        code.addAsmLine(functionEnd + ":");
        return ASTGen::NORETURN;
    
    }
    case RETURN_STATEMENT:
    {
        std::string functionEnd = "mainEnd";
        code.addAsmLine("j " + functionEnd);
        return ASTGen::NORETURN;
    }
    case COMPOSITE:
    {
        for (auto &child : children)
        {
            if(child==nullptr) continue;
            child->generateCode(code);
        }
        return ASTGen::NORETURN;
    }
    case ID:
    {
        if (code.symbolTable.isSymbolExist(id))
        {
            return code.symbolTable.getSymbol(id);
        }
        // for C all variables are declared before use, so this should not happen
        // raise runtime error
        throw std::runtime_error("Symbol not found");
        
    }
    case INTLITERAL:
    {
        code.int2Reg(intVal, reg1);
        SymEntry temp = code.symbolTable.declareTempSymbol();
        code.reg2Sym(Register(reg1), temp);
        return temp;
    }
    case ASSIGN_STATEMENT:
    {
        SymEntry rightSym = children[1]->generateCode(code);
        if(children[1]->nodeType == ARRAY_ACCESS) code.mem2Sym(rightSym, rightSym);
        SymEntry leftSym = children[0]->generateCode(code);
        if(children[2]->nodeType == ARRAY_ACCESS)
        {
            // store value in rightSym to the address that leftSym points to
            code.sym2Reg(leftSym, reg3); // reg3 is the address
            code.sym2Reg(rightSym, reg4); // reg4 is the value
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "sw", reg4.c_str(), reg3.c_str(), "0"));
        }
        else
        {
            code.sym2Sym(leftSym, rightSym); // code::sym2Sym(dest,src)
        }
        
        if (code.symbolTable.isEntTemp(leftSym))
            code.symbolTable.freeTempSymbol(leftSym);
        if (code.symbolTable.isEntTemp(rightSym))
            code.symbolTable.freeTempSymbol(rightSym);
        return ASTGen::NORETURN;
    }
    
    case EXPRESSION:
    {
        if (children.size()==1)
        {
            return children[0]->generateCode(code);
        }
        SymEntry rightSym = children[1]->generateCode(code);
        SymEntry leftSym = children[0]->generateCode(code);
        if(children[1]->nodeType == ARRAY_ACCESS) code.mem2Sym(rightSym, rightSym);
        if(children[0]->nodeType == ARRAY_ACCESS) code.mem2Sym(leftSym, leftSym);
        code.sym2Reg(leftSym, reg2);
        code.sym2Reg(rightSym, reg3);
        if (code.symbolTable.isEntTemp(leftSym))
            code.symbolTable.freeTempSymbol(leftSym);
        if (code.symbolTable.isEntTemp(rightSym))
            code.symbolTable.freeTempSymbol(rightSym);
        if(op=="lt")
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "slt", reg1.c_str(), reg2.c_str(), reg3.c_str()));
        }
        else if (op=="gt")
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "slt", reg1.c_str(), reg3.c_str(), reg2.c_str()));
        }
        else if (op=="lte")
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "slt", reg1.c_str(), reg3.c_str(), reg2.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "xori", reg1.c_str(), reg1.c_str(), "1"));
        }
        else if (op=="gte")
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "slt", reg1.c_str(), reg2.c_str(), reg3.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "xori", reg1.c_str(), reg1.c_str(), "1"));
        }
        else if (op=="eq")
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "xor", reg1.c_str(), reg2.c_str(), reg3.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "sltu", reg1.c_str(), reg1.c_str(), "1"));
        }
        else if (op=="andand")
        {
            // we implemented short-circuit evaluation for &&
            auto andEnd = code.getLabel("andEnd");
            auto andFalse = code.getLabel("andFalse");
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "beq", reg2.c_str(), "$zero", andFalse.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "beq", reg3.c_str(), "$zero", andFalse.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "addi", reg1.c_str(), "$zero", "1"));
            code.addAsmLine(::stringFormat("%s %s,%s", "j", andEnd.c_str()));
            code.addAsmLine(::stringFormat("%s:", andFalse.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "addi", reg1.c_str(), "$zero", "0"));
            code.addAsmLine(::stringFormat("%s:", andEnd.c_str()));
        }
        else if (op=="oror")
        {
            // we implemented short-circuit evaluation for ||
            auto orEnd = code.getLabel("orEnd");
            auto orTrue = code.getLabel("orTrue");
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "bne", reg2.c_str(), "$zero", orTrue.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "bne", reg3.c_str(), "$zero", orTrue.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "addi", reg1.c_str(), "$zero", "0"));
            code.addAsmLine(::stringFormat("%s %s,%s", "j", orEnd.c_str()));
            code.addAsmLine(::stringFormat("%s:", orTrue.c_str()));
            code.addAsmLine(::stringFormat("%s %s,%s,%s", "addi", reg1.c_str(), "$zero", "1"));
            code.addAsmLine(::stringFormat("%s:", orEnd.c_str()));
        }
        else
        {
            code.addAsmLine(::stringFormat("%s %s,%s,%s", op.c_str(), reg1.c_str(), reg2.c_str(), reg3.c_str()));
        }
        
        
        
        SymEntry temp = code.symbolTable.declareTempSymbol();
        code.reg2Sym(reg1, temp);
        return temp;
    }

    case UNARY_EXPRESSION:
    {

        

            SymEntry rightSym = children[0]->generateCode(code);
            if(children[0]->nodeType == ARRAY_ACCESS) code.mem2Sym(rightSym, rightSym);
            code.sym2Reg(rightSym, reg1);
            if (code.symbolTable.isEntTemp(rightSym))
                code.symbolTable.freeTempSymbol(rightSym);
            if(op=="not")
            {
                // logical not of $t1: sltiu $t2, $t1, 1
                code.addAsmLine(::stringFormat("%s %s,%s,%s", "sltiu", reg2.c_str(), reg1.c_str(), "1"));
            }
            else
            {
                code.addAsmLine(::stringFormat("%s %s,%s", op.c_str(), reg2.c_str(), reg1.c_str()));
            }
            SymEntry temp = code.symbolTable.declareTempSymbol();
            code.reg2Sym(reg2, temp);
            return temp;
        
        
    }
    case WHILE_STATEMENT:
    {
        std::string whileLabel = code.getLabel("whileStart");
        std::string endLabel = code.getLabel("whileEnd");
        code.addAsmLine(::stringFormat("%s:", whileLabel.c_str()));
        SymEntry condSym = children[0]->generateCode(code);
        if (children[0]->nodeType == ARRAY_ACCESS)
            code.mem2Sym(condSym, condSym);
        code.sym2Reg(condSym, reg1);
        code.addAsmLine(::stringFormat("beq %s,$zero,%s", reg1.c_str(), endLabel.c_str()));
        children[1]->generateCode(code);
        code.addAsmLine(::stringFormat("j %s", whileLabel.c_str()));
        code.addAsmLine(::stringFormat("%s:", endLabel.c_str()));
        if (code.symbolTable.isEntTemp(condSym))
            code.symbolTable.freeTempSymbol(condSym);
        return ASTGen::NORETURN;
    }
    case DO_WHILE_STATEMENT:
    {
        std::string whileLabel = code.getLabel("doWhileStart");
        std::string endLabel = code.getLabel("doWhileEnd");
        code.addAsmLine(::stringFormat("%s:", whileLabel.c_str()));
        children[0]->generateCode(code);
        SymEntry condSym = children[1]->generateCode(code);
        if (children[1]->nodeType == ARRAY_ACCESS)
            code.mem2Sym(condSym, condSym);
        code.sym2Reg(condSym, reg1);
        code.addAsmLine(::stringFormat("beq %s,$zero,%s", reg1.c_str(), endLabel.c_str()));
        code.addAsmLine(::stringFormat("j %s", whileLabel.c_str()));
        code.addAsmLine(::stringFormat("%s:", endLabel.c_str()));
        if (code.symbolTable.isEntTemp(condSym))
            code.symbolTable.freeTempSymbol(condSym);
        return ASTGen::NORETURN;
    }
    case IF_STATEMENT:
    {
        // the production rule:
        // if_statement -> if_stmt
        // if_statement -> if_stmt ELSE code_block
        // if_stmt -> IF LPAR exp RPAR code_block
        // now implement according to the production rule
        std::string elseLabel = code.getLabel("else");
        std::string endLabel = code.getLabel("end");
        SymEntry condSym = children[0]->generateCode(code);
        if (children[0]->nodeType == ARRAY_ACCESS)
            code.mem2Sym(condSym, condSym);
        code.sym2Reg(condSym, reg1);
        code.addAsmLine(::stringFormat("beq %s,$zero,%s", reg1.c_str(), elseLabel.c_str()));
        children[1]->generateCode(code);
        code.addAsmLine(::stringFormat("j %s", endLabel.c_str()));
        code.addAsmLine(::stringFormat("%s:", elseLabel.c_str()));
        if (children.size() == 3)
        {
            children[2]->generateCode(code);
        }
        code.addAsmLine(::stringFormat("%s:", endLabel.c_str()));
        if (code.symbolTable.isEntTemp(condSym))
            code.symbolTable.freeTempSymbol(condSym);
        return ASTGen::NORETURN;


    }
    case ARRAY_ACCESS:
    {
        SymEntry arraySym = children[0]->generateCode(code);
        SymEntry indexSym = children[1]->generateCode(code);
        if (children[1]->nodeType == ARRAY_ACCESS)
            code.mem2Sym(indexSym, indexSym);
        code.getAddressOf(arraySym, reg1); // put base address of array in reg1
        code.sym2Reg(indexSym, reg2);
        code.addAsmLine(::stringFormat("sll %s,%s,2", reg2.c_str(), reg2.c_str()));
        code.addAsmLine(::stringFormat("add %s,%s,%s", reg1.c_str(), reg1.c_str(), reg2.c_str()));
        SymEntry tempPointer = code.symbolTable.declareTempSymbol();
        code.reg2Sym(reg1, tempPointer);
        if (code.symbolTable.isEntTemp(arraySym))
            code.symbolTable.freeTempSymbol(arraySym);
        if (code.symbolTable.isEntTemp(indexSym))
            code.symbolTable.freeTempSymbol(indexSym);
        return tempPointer; // this is a pointer to the array element, not the value!
        
    }
    case READ_STATEMENT:
    {
        SymEntry readSym = children[0]->generateCode(code);
        code.sysRead(readSym);
        return ASTGen::NORETURN;
    }
    case WRITE_STATEMENT:
    {
        SymEntry writeSym = children[0]->generateCode(code);
        code.sysWrite(writeSym);
        return ASTGen::NORETURN;
    }
    default:
        break;
    }
    return NORETURN;
}

void ASTGen::SyntaxTreeNode::addChild(std::shared_ptr<SyntaxTreeNode> child)
{
    children.push_back(child);
}