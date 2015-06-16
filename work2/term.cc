#include <term.h>

std::string Term::box(char c)
{
    return std::string("\e(0") + c + "\e(B";
}

std::string Term::blue(std::string s)
{
    return "\e[1;34m" + s + "\e[0m";
}

std::string Term::cyan(std::string s)
{
    return "\e[1;36m" + s + "\e[0m";
}

std::string Term::green(std::string s)
{
    return "\e[1;32m" + s + "\e[0m";
}
