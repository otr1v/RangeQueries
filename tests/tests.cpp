#ifdef DEBUG
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include "avl_tree.hpp" // Предполагается, что avl_tree::AVLTree определен здесь

// using namespace avl_tree;

// Тестовый класс для avl_tree::AVLTree
class AVLTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация перед каждым тестом
    }

    // Вспомогательная функция для проверки фактора баланса
    template<typename T, typename Compare = std::less<T>>
    bool VerifyBalance(const avl_tree::AVLTree<T, Compare>& tree) {
        // Внутренняя функция для доступа к приватным членам
        class BalanceChecker {
        public:
            static bool Check(const std::unique_ptr<typename avl_tree::AVLTree<T, Compare>::Node>& node) {
                if (!node) return true;
                int leftHeight = node->left_ ? node->left_->height_ : 0;
                int rightHeight = node->right_ ? node->right_->height_ : 0;
                int balance = leftHeight - rightHeight;
                if (balance < -1 || balance > 1) return false;
                return Check(node->left_) && Check(node->right_);
            }
        };
        return BalanceChecker::Check(tree.*&avl_tree::AVLTree<T, Compare>::root_);
    }

    // Вспомогательная функция для получения высоты дерева
    template<typename T, typename Compare = std::less<T>>
    int GetTreeHeight(const avl_tree::AVLTree<T, Compare>& tree) {
        class HeightChecker {
        public:
            static int Get(const std::unique_ptr<typename avl_tree::AVLTree<T, Compare>::Node>& node) {
                if (!node) return 0;
                return node->height_;
            }
        };
        return HeightChecker::Get(tree.*&avl_tree::AVLTree<T, Compare>::root_);
    }
};

// Тест 1: Проверка вставки и свойства BST (int)
TEST_F(avl_tree::AVLTreeTest, InsertAndBSTProperty_Int) {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(20);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(30);

    std::vector<int> result;
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }

    std::vector<int> expected = {5, 10, 15, 20, 30};
    EXPECT_EQ(result, expected);
}

// Тест 2: Проверка балансировки AVL
TEST_F(avl_tree::AVLTreeTest, BalanceProperty) {
    avl_tree::AVLTree<int> tree;
    // Вставка в порядке, вызывающем вращения
    tree.Insert(3);
    tree.Insert(2);
    tree.Insert(1); // Требуется правое вращение (LL)

    EXPECT_TRUE(VerifyBalance(tree));
    EXPECT_EQ(GetTreeHeight(tree), 2); // Высота должна быть log(3) ~ 2

    tree.Insert(4);
    tree.Insert(5); // Требуется левое вращение (RR)

    EXPECT_TRUE(VerifyBalance(tree));
    EXPECT_EQ(GetTreeHeight(tree), 3); // Высота должна быть log(5) ~ 3

    // Тест на LR случай
    avl_tree::AVLTree<int> tree2;
    tree2.Insert(3);
    tree2.Insert(1);
    tree2.Insert(2); // Требуется лево-правое вращение (LR)
    EXPECT_TRUE(VerifyBalance(tree2));

    // Тест на RL случай
    avl_tree::AVLTree<int> tree3;
    tree3.Insert(1);
    tree3.Insert(3);
    tree3.Insert(2); // Требуется право-левое вращение (RL)
    EXPECT_TRUE(VerifyBalance(tree3));
}

// Тест 3: Проверка обработки дубликатов
TEST_F(avl_tree::AVLTreeTest, DuplicateKeys) {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(10); // Дубликат
    tree.Insert(20);

    std::vector<int> result;
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }

    std::vector<int> expected = {10, 20};
    EXPECT_EQ(result, expected);
}

// Тест 4: Проверка pre-order обхода
TEST_F(avl_tree::AVLTreeTest, PreOrderTraversal) {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(3);
    tree.Insert(7);

    std::vector<int> result;
    for (auto it = tree.BeginPreOrder(); it != tree.EndPreOrder(); ++it) {
        result.push_back(*it);
    }

    std::vector<int> expected = {10, 5, 3, 7, 20}; // NLR
    EXPECT_EQ(result, expected);
}

// Тест 5: Проверка post-order обхода
TEST_F(avl_tree::AVLTreeTest, PostOrderTraversal) {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(3);
    tree.Insert(7);

    std::vector<int> result;
    for (auto it = tree.BeginPostOrder(); it != tree.EndPostOrder(); ++it) {
        result.push_back(*it);
    }

    std::vector<int> expected = {3, 7, 5, 20, 10}; // LRN
    EXPECT_EQ(result, expected);
}

// Тест 6: Проверка range queries
TEST_F(avl_tree::AVLTreeTest, RangeQuery) {
    avl_tree::AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(20);
    tree.Insert(15);
    tree.Insert(30);
    tree.Insert(3);
    tree.Insert(7);

    std::vector<int> result;
    tree.RangeQuery(5, 20, [&result](int key) {
        result.push_back(key);
    });

    std::vector<int> expected = {5, 7, 10, 15, 20};
    EXPECT_EQ(result, expected);

    // Пустой диапазон
    result.clear();
    tree.RangeQuery(25, 28, [&result](int key) {
        result.push_back(key);
    });
    EXPECT_TRUE(result.empty());

    // Диапазон с одним элементом
    result.clear();
    tree.RangeQuery(10, 10, [&result](int key) {
        result.push_back(key);
    });
    expected = {10};
    EXPECT_EQ(result, expected);
}

// Тест 7: Проверка пустого дерева
TEST_F(avl_tree::AVLTreeTest, EmptyTree) {
    avl_tree::AVLTree<int> tree;

    std::vector<int> result;
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }
    EXPECT_TRUE(result.empty());

    result.clear();
    tree.RangeQuery(1, 10, [&result](int key) {
        result.push_back(key);
    });
    EXPECT_TRUE(result.empty());
}

// Тест 8: Проверка шаблонизации со std::string
TEST_F(avl_tree::AVLTreeTest, TemplateWithString) {
    avl_tree::AVLTree<std::string> tree;
    tree.Insert("banana");
    tree.Insert("apple");
    tree.Insert("cherry");

    std::vector<std::string> result;
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }

    std::vector<std::string> expected = {"apple", "banana", "cherry"};
    EXPECT_EQ(result, expected);

    // Проверка range query
    result.clear();
    tree.RangeQuery("apple", "banana", [&result](const std::string& key) {
        result.push_back(key);
    });
    expected = {"apple", "banana"};
    EXPECT_EQ(result, expected);

    EXPECT_TRUE(VerifyBalance(tree));
}

// Тест 9: Проверка высоты для большого дерева
TEST_F(avl_tree::AVLTreeTest, LargeTreeHeight) {
    avl_tree::AVLTree<int> tree;
    const int n = 1000;
    for (int i = 1; i <= n; ++i) {
        tree.Insert(i);
    }

    int height = GetTreeHeight(tree);
    // Высота должна быть около log2(n) + 1
    EXPECT_LE(height, 1.44 * std::log2(n) + 2); // Приблизительная оценка

    EXPECT_TRUE(VerifyBalance(tree));

    std::vector<int> result;
    for (auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }
    std::vector<int> expected(n);
    std::iota(expected.begin(), expected.end(), 1);
    EXPECT_EQ(result, expected);
}

// Основная функция для запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif