#include "avl_tree.hpp"
#include "tree_exceptions.hpp"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    avl_tree::AVLTree<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);
    tree.Insert(60);
    tree.Insert(80);

    std::cout << "Pre-order traversal: ";
    for (auto it = tree.BeginPreOrder(); it != tree.EndPreOrder(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "In-order traversal: ";
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it)
    {
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
    // TODO catch exceptions
    
    
    std::cout << "Keys in range [30, 60]: " << tree.RangeQuery(30, 60);;

    
    std::cout << std::endl;
    google::ShutdownGoogleLogging();

    return 0;
}