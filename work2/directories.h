#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <vector>
#include <printer.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class DirEntry
{
    std::string path, name;
    struct stat st;
    
    DirEntry(std::string path, std::string name);
    
public:
    static DirEntry *getEntry(std::string path, std::string name);

    bool isDir();
    bool isLink();
    bool isExecutable();
    std::string readLink();
    std::string getName();
    std::string getPath();
    off_t size();
    DirEntry *getChild(std::string name);
    std::vector<DirEntry *> getChildren();
    void print(TreePrinter& p);
};

#endif
