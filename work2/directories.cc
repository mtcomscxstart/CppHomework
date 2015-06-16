#include <directories.h>
#include <term.h>
#include <dirent.h>

#include <algorithm>

std::string upper(std::string s)
{
    std::string res;
    for (auto ch : s)
        res += toupper(ch);
    return res;
}

bool cmp(DirEntry *a, DirEntry *b)
{
    if (a->isDir() && !b->isDir())
        return true;
    if (!a->isDir() && b->isDir())
        return false;
    std::string aName = upper(a->getName()),
                bName = upper(b->getName());
    return aName.compare(bName) < 0;
}

DirEntry::DirEntry(std::string dirpath, std::string dirname)
{
    path = dirpath;
    name = dirname;
    lstat(path.c_str(), &st);
}

DirEntry *DirEntry::getEntry(std::string path, std::string name)
{
    if (access(path.c_str(), R_OK) == 0)
        return new DirEntry(path, name);
    return nullptr;
}

bool DirEntry::isDir()
{
    return S_ISDIR(st.st_mode);
}

bool DirEntry::isLink()
{
    return S_ISLNK(st.st_mode);
}

bool DirEntry::isExecutable()
{
    return (st.st_mode & S_IXUSR) != 0;
}

off_t DirEntry::size()
{
    return st.st_size;
}

std::string DirEntry::readLink()
{
    if (!isLink())
        return "";
    char *s = new char[size() + 1];
    readlink(path.c_str(), s, size());
    s[size()] = '\0';
    std::string res(s);
    delete[] s;
    return res;
}

std::string DirEntry::getName()
{
    return name;
}

std::string DirEntry::getPath()
{
    return path;
}

DirEntry *DirEntry::getChild(std::string name)
{
    std::string npath = path;
    if (npath.back() != '/')
        npath += '/';
    return DirEntry::getEntry(npath + name, name);
}

std::vector<DirEntry*> DirEntry::getChildren()
{
    std::vector<DirEntry*> c;
    if (isDir()) {
        DIR *dir = opendir(path.c_str());
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string chname = entry->d_name;
            if (chname != "." && chname != "..")
                c.push_back(getChild(chname));
        }
    }
    std::sort(c.begin(), c.end(), cmp);
    return c;
}
