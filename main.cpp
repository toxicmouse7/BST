#include <iostream>
#include <vector>
#include "BST.hpp"

int main()
{
    BST<int, int> tree;
    BST<int, int> tree2;

    tree.insert(4, 1);
    tree.insert(2, 2);
    tree.insert(1, 3);
    tree.insert(5, 4);
    tree.insert(3, 5);

    tree2.insert(5,4);
    tree2.insert(4,1);
    tree2.insert(3,5);
    tree2.insert(2,2);
    tree2.insert(1,3);

    std::cout << tree.isKeySetEqualTo(tree2) << std::endl;


    return 0;
}
