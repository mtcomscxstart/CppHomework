#include <directories.h>
#include <statprinter.h>

int main(int argc, char *argv[])
{
    std::string path(".");
    if (argc >= 2)
        path = std::string(argv[1]);
    DirEntry *e = DirEntry::getEntry(path, "");
    StatPrinter s;
    s.printStat(e);
    return 0;
}
