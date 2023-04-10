#include "production.h"
#include <cassert>

Production::Production(Symbol lhs, std::vector<Symbol> rhs, int id) : id(id), lhs(lhs), rhs(rhs) {
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