#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include "BST.hpp"

int main()
{
    std::vector<int> keys(10'000'000);
    std::iota(keys.begin(), keys.end(), 1);

    std::shuffle(keys.begin(), keys.end(), std::random_device());

    BST<int, int> tree1;
    for (auto& k : keys)
        tree1.insert(k, 0);

    std::shuffle(keys.begin(), keys.end(), std::random_device());

    BST<int, int> tree2;
    for (auto& k : keys)
        tree2.insert(k, 0);

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << tree1.isKeySetEqualToAsync(tree2) << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << tree1.isKeySetEqualTo(tree2) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(end - start).count() << std::endl;

    return 0;
}
