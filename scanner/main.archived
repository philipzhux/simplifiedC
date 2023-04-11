#include "scanner.hpp"
#include <iostream>

int main()
{
    std::string src = "";
    for (std::string line; std::getline(std::cin, line);) {
        src += line+"\n";
    }
    Scanner lex(src);
    if(!lex.compile())
    {
        std::cout<<"Syntax Error"<<std::endl;
    }
    else
    {
        for(auto &t: lex.matchedTokens)
        {
             std::cout<<"Token: "<<TokenMapper::token_to_string[t]<<std::endl;
        }
       
    }

}