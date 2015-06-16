#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

class TreePrinter
{
    std::vector<int> left;

    void prologue();
    void epilogue();
public:
    void put(std::string s);
    void enter(int k);
};

#endif
