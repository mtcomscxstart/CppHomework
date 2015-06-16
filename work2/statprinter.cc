#include <statprinter.h>
#include <term.h>

#include <iostream>

const char *sz_names[] = {"B", "KB", "MB", "GB"};

void StatPrinter::printStat(DirEntry *d)
{
    fl = dr = ln = sz = 0;
    t = TreePrinter();
    print(d);
    std::cout << "Directories: " << dr - 1 << std::endl
              << "Files: " << fl << std::endl
              << "Links: " << ln << std::endl;
              
    int v = 0;
    while (sz >= 1024) {
        sz /= 1024;
        v++;
    }
    
    std::cout << "Size: " << sz << " " << sz_names[v] << std::endl;
}

void StatPrinter::print(DirEntry *d)
{
    std::string pretty;
    
    if (d->getName().empty())
        pretty = d->getPath();
    else
        pretty = d->getName();
        
    if (d->isDir()) {
        pretty = Term::blue(pretty);
        dr++;
    } else if (d->isLink()) {
        pretty = Term::cyan(pretty) + " -> " + d->readLink();
        ln++;
    } else if (d->isExecutable()) {
        pretty = Term::green(pretty);
        fl++;
    } else
        fl++;
    
    sz += d->size();

    t.put(pretty);

    std::vector<DirEntry*> ch = d->getChildren();
    t.enter(ch.size());
    for (auto i : ch)
        print(i);
}
