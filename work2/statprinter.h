#ifndef STATPRINTER_H
#define STATPRINTER_H

#include <directories.h>

class StatPrinter
{
    int fl, dr, ln, sz;
    TreePrinter t;
    
    void print(DirEntry *d);
public:
    void printStat(DirEntry *d);
};

#endif
