#include <iostream>
#include <vector>
#include "BST.hpp"

int main()
{
    BST<int, int> tree;

    tree.insert(13, 0);
    tree.insert(8, 0);
    tree.insert(17, 0);
    tree.insert(1, 0);
    tree.insert(11, 0);
    tree.insert(6, 1);
    tree.insert(15, 0);
    tree.insert(25, 1);
    tree.insert(22, 0);
    tree.insert(27, 0);

    std::cout << tree.isRBT();

    //tree.erase(3);



    return 0;
}
