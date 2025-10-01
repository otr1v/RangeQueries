#include "avl_tree.hpp"
#include "tree_exceptions.hpp"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = google::FATAL;
    avl_tree::AVLTree<int> tree;

    char command;

    try {
        while (std::cin >> command) {
            switch (command) {
                case 'k': {
                    int n;
                    std::cin >> n;
                    if (!std::cin.good()) {
                        throw std::invalid_argument("\n Key invalid");
                    }

                    tree.Insert(n);
                    break;
                }
                case 'q': {
                    int a;
                    std::cin >> a;
                    if (!std::cin.good()) {
                        throw std::invalid_argument(
                            "\n Invalid first number for the request");
                    }

                    int b;
                    std::cin >> b;
                    if (!std::cin.good()) {
                        throw std::invalid_argument(
                            "\n Invalid second number for the request");
                    }

                    std::cout << tree.RangeQuery(a, b) << " ";
                    break;
                }
                default:
                    throw std::invalid_argument("\n Unknown command");
            }
        }
        std::cout << std::endl;
    } catch (const avl_tree::AVLException &e) {
        std::cerr << "Tree error: " << e.what() << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Input error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Don't know this exception" << std::endl;
    }

    google::ShutdownGoogleLogging();

    return 0;
}