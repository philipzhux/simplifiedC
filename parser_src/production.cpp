#include "production.h"
#include <cassert>

Production::Production() {}

Production::Production(Symbol lhs, std::vector<Symbol> rhs, int id, std::function<std::shared_ptr<ASTGen::SyntaxTreeNode>(std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>>)> action) : id(id), lhs(lhs), rhs(rhs), action(action) {
    assert(!lhs.isTerminal);
}

bool Production::operator==(const Production &other) const
{
    return id == other.id;
}

int Production::getId()
{
    static int i = 0;
    return i++;
}