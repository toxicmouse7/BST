#include <iostream>
#include <vector>
#include "BST.hpp"

int main()
{
    BST<int, int> tree1;
    BST<int, int> tree2;

    tree1.insert(5, 0);
    tree1.insert(1, 0);
    tree1.insert(4, 0);
    tree1.insert(2, 0);
    tree1.insert(3, 0);
    tree1.insert(0, 0);

    tree2.insert(5, 0);
    tree2.insert(1, 0);
    tree2.insert(4, 0);
    tree2.insert(2, 0);
    tree2.insert(3, 0);
    tree2.insert(0, 0);

    std::cout << tree1.isStructureEqual(tree2);



    return 0;
}
