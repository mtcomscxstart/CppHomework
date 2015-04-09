#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

const char tree_fork = 0x74,
           tree_corner = 0x6d,
           tree_vert = 0x78,
           tree_horiz = 0x71;

bool set[256];
int f_count = 0, 
    d_count = 0,
    ln_count = 0;

const char *sz_names[] = {"B", "KB", "MB", "GB"};

off_t bytes = 0;

struct stat get_stat(const char *path);
bool is_dir(const char *path);
bool is_link(const char *path);
off_t get_size(const char *path);
void count(const char *path);
std::string link_content(const char *path);
int dirfilter(const struct dirent *d);
int dirsort(const struct dirent **a,
            const struct dirent **b);
void print_tree(const char *path);
void print_tree_recursive(const char *path,
                          bool end, int l);
std::string escape_box_drawing(char c);
std::string escape_blue(std::string);
void pretty_print_path(const char *path,
                       bool end, int l);
void print_stat();

const std::string s_tree_corner = escape_box_drawing(tree_corner),
                  s_tree_fork = escape_box_drawing(tree_fork),
                  s_tree_vert = escape_box_drawing(tree_vert),
                  s_tree_horiz = escape_box_drawing(tree_horiz);

int main(int argc, char *argv[])
{
    char default_path[] = ".", *path;
    
    if (argc >= 2)
        path = argv[1];
    else
        path = default_path;
    
    print_tree(path);
    print_stat();
    
    return EXIT_SUCCESS;
}

struct stat get_stat(const char *path)
{
    struct stat buf;

    lstat(path, &buf);
    return buf;
}

bool is_dir(const char *path)
{
    return S_ISDIR(get_stat(path).st_mode);
}

bool is_link(const char *path)
{
    return S_ISLNK(get_stat(path).st_mode);
}

off_t get_size(const char *path)
{
    struct stat st = get_stat(path);
    return st.st_size;
}

void count(const char *path)
{
    if (is_dir(path))
        d_count++;
    else if (is_link(path))
        ln_count++;
    else {
        f_count++;
        bytes += get_size(path);
    }
}

std::string link_content(const char *path)
{
    off_t size = get_size(path);
    char *s = new char[size + 1];
    
    readlink(path, s, size + 1);
    
    std::string res(s);
    delete[] s;
    
    return res;
}

int dirsort(const struct dirent **a,
            const struct dirent **b)
{
    bool isdir_a, isdir_b;
    
    isdir_a = is_dir((*a)->d_name);
    isdir_b = is_dir((*b)->d_name);
    
    if (isdir_a && !isdir_b)
        return 1;
    if (!isdir_a && isdir_b)
        return -1;
    return alphasort(a, b);
}

int dirfilter(const struct dirent *d)
{
    std::string name = d->d_name;
    return name != "." &&
           name != "..";
}

void print_tree(const char *path)
{
    print_tree_recursive(path, true, 0);
}

void print_tree_recursive(const char *path,
                          bool end, int l)
{
    struct dirent **dirents;
    int n;
    
    pretty_print_path(path, end, l);
    
    if (!is_dir(path) || is_link(path))
        return;
    
    chdir(path);
    n = scandir(".", &dirents, dirfilter, dirsort);
    if (n < 0)
        std::cerr << "Can't read directory" << std::endl;
    else {
        set[l] = true;
        for (int i = 0; i < n; i++) {
            bool is_end = i == n - 1;
            
            if (is_end)
                set[l] = false;
                
            count(dirents[i]->d_name);
            print_tree_recursive(dirents[i]->d_name, is_end, l + 1);
            
            /* free malloc'd memory, not delete */
            free(dirents[i]);
        }
        free(dirents);
    }
    chdir("..");
}

std::string escape_box_drawing(char c)
{
    return std::string("\e(0") + c + "\e(B";
}

std::string escape_blue(std::string s)
{
    return "\e[1;34m" + s + "\e[0m";
}

void pretty_print_path(const char *path, bool end, int l)
{
    std::string s_path = path;
    for (int i = 0; i < l; i++) {
        if (i == l - 1) {
            std::cout << (end ? s_tree_corner : s_tree_fork)
                    << s_tree_horiz << s_tree_horiz;
        } else if (set[i])
            std::cout << s_tree_vert << "  ";
        else
            std::cout << "   ";
        std::cout << " ";
    }
    std::cout << (is_dir(path) ? escape_blue(s_path) : s_path)
              << (is_link(path) ? " -> " + link_content(path) : "")
              << std::endl;
}

void print_stat()
{
    std::cout << "Files: " << f_count << std::endl
              << "Directories: " << d_count << std::endl
              << "Links: " << ln_count << std::endl;
    int k = 0, sz = bytes;
    while (sz >= 1024) {
        sz >>= 10;
        k++;
    }
    std::cout << "Total size: " << sz << sz_names[k] << std::endl;
}
