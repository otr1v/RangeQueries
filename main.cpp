#include "avl_tree.hpp"
#include "tree_exceptions.hpp"


int main() {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(20);
    tree.Insert(30);

    
    std::cout << "Pre-order traversal: ";
    for (auto it = tree.BeginPreOrder(); it != tree.EndPreOrder(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    
    std::cout << "In-order traversal: ";
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // FIXME
/*
    // Post-order обход
    std::cout << "Post-order traversal: ";
    for (auto it = tree.BeginPostOrder(); it != tree.EndPostOrder(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
*/
    
    std::vector<int> results;
    tree.RangeQuery(15, 25, [&results](int key) {
        results.push_back(key); // FIXME сделать резервацию для вектора заранее 
    });
    std::cout << "Keys in range [15, 25]: ";
    for (int key : results) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    return 0;
}