#ifndef TERM_H
#define TERM_H

#include <string>

class Term
{
public:
    static std::string box(char c);
    static std::string blue(std::string s);
    static std::string cyan(std::string s);
    static std::string green(std::string s);
};

#endif
