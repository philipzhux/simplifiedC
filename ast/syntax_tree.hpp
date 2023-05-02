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
    enum NodeType
    {
        POINTER_ASSIGN_STATEMENT,
        DECLARATION,
        ASSIGN_STATEMENT,
        INTLITERAL,
        UNARY_EXPRESSION,
        EXPRESSION,
        ID,
        READ_STATEMENT,
        WRITE_STATEMENT,
        IF_STATEMENT,
        WHILE_STATEMENT,
        ARRAY_ACCESS,
        DO_WHILE_STATEMENT,
        RETURN_STATEMENT,
        FUNCTION,
        COMPOSITE
    };

    class SyntaxTreeNode
    {
    public:
        SyntaxTreeNode() = delete;
        SyntaxTreeNode(NodeType, std::string); // for expr and id
        SyntaxTreeNode(NodeType, int);
        SyntaxTreeNode(NodeType);
        NodeType nodeType;
        std::string id; // symbol key for identifiers
        int intVal;     // value for int literals
        std::string op; // for add or sub
        SymEntry generateCode(Code &code);
        void addChild(std::shared_ptr<SyntaxTreeNode>);
        std::vector<std::shared_ptr<SyntaxTreeNode>> children;
    };

};
