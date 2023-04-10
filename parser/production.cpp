#include "production.h"

Production::Production(Symbol lhs, std::vector<Symbol> rhs, int id) : id(id), lhs(lhs), rhs(rhs) {
    assert(!lhs.isTerminal);
}

bool Production::operator==(const Production &other) const
{
    return id == other.id;
}

