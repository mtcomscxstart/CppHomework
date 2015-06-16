#include <directories.h>
#include <term.h>
#include <charconst.h>
#include <iostream>

void TreePrinter::enter(int k)
{
    left.push_back(k);
}

void TreePrinter::put(std::string s)
{
    prologue();
    std::cout << s;
    epilogue();
}

void TreePrinter::prologue()
{
    while (!left.empty() && left.back() == 0)
        left.pop_back();
    if (!left.empty()) {
        left.back()--;
        for (int i = 0; i < left.size() - 1; i++) {
            if (left[i])
                std::cout << Term::box(tree_vert) << "  ";
            else
                std::cout << "   ";
            std::cout << " ";
        }
        std::cout << (left.back() > 0 ?
                        Term::box(tree_fork) :
                        Term::box(tree_corner))
                  << Term::box(tree_horiz)
                  << Term::box(tree_horiz)
                  << " ";
    }
}

void TreePrinter::epilogue()
{
    std::cout << std::endl;
}
