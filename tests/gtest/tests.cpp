#include <limits>
#include <string>
#include <vector>

#include "avl_tree.hpp"
#include "gtest/gtest.h"

namespace avl_tree {

TEST(AVLTreeRangeQueryIntTest, EmptyTree) {
    AVLTree<int> tree;
    EXPECT_EQ(tree.RangeQuery(0, 100), 0);
    EXPECT_EQ(tree.RangeQuery(-100, 100), 0);
    EXPECT_EQ(tree.RangeQuery(std::numeric_limits<int>::min(),
                              std::numeric_limits<int>::max()),
              0);
}

TEST(AVLTreeRangeQueryIntTest, SingleElementTree) {
    AVLTree<int> tree;
    tree.Insert(10);

    EXPECT_EQ(tree.RangeQuery(0, 5), 0);
    EXPECT_EQ(tree.RangeQuery(15, 20), 0);
    EXPECT_EQ(tree.RangeQuery(11, 11), 0);

    EXPECT_EQ(tree.RangeQuery(0, 10), 1);
    EXPECT_EQ(tree.RangeQuery(10, 20), 1);
    EXPECT_EQ(tree.RangeQuery(5, 15), 1);
    EXPECT_EQ(tree.RangeQuery(10, 10), 1);
}

TEST(AVLTreeRangeQueryIntTest, SimpleTreeThreeNodes) {
    AVLTree<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    // Дерево:
    //     10
    //    /  \
    //   5   15

    EXPECT_EQ(tree.RangeQuery(0, 20), 3);
    EXPECT_EQ(tree.RangeQuery(5, 15), 3);
    EXPECT_EQ(tree.RangeQuery(6, 14), 1);
    EXPECT_EQ(tree.RangeQuery(5, 10), 2);
    EXPECT_EQ(tree.RangeQuery(10, 15), 2);
    EXPECT_EQ(tree.RangeQuery(0, 4), 0);
    EXPECT_EQ(tree.RangeQuery(16, 20), 0);
    EXPECT_EQ(tree.RangeQuery(6, 9), 0);
    EXPECT_EQ(tree.RangeQuery(11, 14), 0);
    EXPECT_EQ(tree.RangeQuery(5, 5), 1);
    EXPECT_EQ(tree.RangeQuery(10, 10), 1);
    EXPECT_EQ(tree.RangeQuery(15, 15), 1);
    EXPECT_EQ(tree.RangeQuery(11, 11), 0);
    EXPECT_EQ(tree.RangeQuery(15, 5), 0);
}

TEST(AVLTreeRangeQueryIntTest, LargerTreeExample) {
    AVLTree<int> tree;
    const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    for (int val : values) {
        tree.Insert(val);
    }
    // Дерево (сбалансированное):
    //       50
    //      /  \
    //     30   70
    //    / \   / \
    //   20 40 60  80
    const size_t total_elements = values.size();

    EXPECT_EQ(tree.RangeQuery(0, 100), total_elements);
    EXPECT_EQ(tree.RangeQuery(20, 80), total_elements);
    EXPECT_EQ(tree.RangeQuery(30, 70), 5);
    EXPECT_EQ(tree.RangeQuery(20, 40), 3);
    EXPECT_EQ(tree.RangeQuery(60, 80), 3);
    EXPECT_EQ(tree.RangeQuery(50, 80), 4);
    EXPECT_EQ(tree.RangeQuery(45, 55), 1);
    EXPECT_EQ(tree.RangeQuery(51, 59), 0);
    EXPECT_EQ(tree.RangeQuery(0, 19), 0);
    EXPECT_EQ(tree.RangeQuery(81, 100), 0);
    EXPECT_EQ(tree.RangeQuery(40, 40), 1);
    EXPECT_EQ(tree.RangeQuery(45, 45), 0);
    EXPECT_EQ(tree.RangeQuery(20, 20), 1);
    EXPECT_EQ(tree.RangeQuery(80, 80), 1);
    EXPECT_EQ(tree.RangeQuery(19, 21), 1);
    EXPECT_EQ(tree.RangeQuery(79, 81), 1);
    EXPECT_EQ(tree.RangeQuery(35, 65), 3);
    EXPECT_EQ(tree.RangeQuery(70, 60), 0);
}

TEST(AVLTreeRangeQueryStringTest, StringData) {
    AVLTree<std::string> tree;
    tree.Insert("banana");
    tree.Insert("apple");
    tree.Insert("cherry");
    tree.Insert("date");
    tree.Insert("fig");
    // Элементы в дереве (in-order): apple, banana, cherry, date, fig

    EXPECT_EQ(tree.RangeQuery("apple", "fig"), 5);
    EXPECT_EQ(tree.RangeQuery("banana", "date"), 3);
    EXPECT_EQ(tree.RangeQuery("blueberry", "cranberry"), 1);
    EXPECT_EQ(tree.RangeQuery("grape", "kiwi"), 0);
    EXPECT_EQ(tree.RangeQuery("fig", "apple"), 0);
    EXPECT_EQ(tree.RangeQuery("cherry", "cherry"), 1);
    EXPECT_EQ(tree.RangeQuery("a", "z"), 5);
}

TEST(AVLTreeRangeQueryComplexTest, LargeSortedAscending) {
    AVLTree<int> tree;
    const int n = 2000;
    for (int i = 1; i <= n; ++i) {
        tree.Insert(i);
    }

    EXPECT_EQ(tree.RangeQuery(1, n), n) << "Диапазон, включающий все элементы";
    EXPECT_EQ(tree.RangeQuery(1, n / 2), n / 2) << "Первая половина элементов";
    EXPECT_EQ(tree.RangeQuery(n / 2 + 1, n), n - (n / 2))
        << "Вторая половина элементов";
    EXPECT_EQ(tree.RangeQuery(n / 4, 3 * n / 4), (3 * n / 4) - (n / 4) + 1)
        << "Средний диапазон";
    EXPECT_EQ(tree.RangeQuery(1, 1), 1) << "Только первый элемент";
    EXPECT_EQ(tree.RangeQuery(n, n), 1) << "Только последний элемент";
    EXPECT_EQ(tree.RangeQuery(n + 1, n + 10), 0)
        << "Диапазон выше всех элементов";
    EXPECT_EQ(tree.RangeQuery(0, 0), 0) << "Диапазон ниже всех элементов";
    EXPECT_EQ(tree.RangeQuery(n / 3, n / 2), (n / 2) - (n / 3) + 1)
        << "Вложенный диапазон";
    EXPECT_EQ(tree.RangeQuery(n, 1), 0) << "Некорректный диапазон max < min";
}

TEST(AVLTreeRangeQueryComplexTest, LargeSortedDescending) {
    AVLTree<int> tree;
    const int n = 2000;
    for (int i = n; i >= 1; --i) {
        tree.Insert(i);
    }

    EXPECT_EQ(tree.RangeQuery(1, n), n);
    EXPECT_EQ(tree.RangeQuery(1, n / 2), n / 2);
    EXPECT_EQ(tree.RangeQuery(n / 2 + 1, n), n - (n / 2));
    EXPECT_EQ(tree.RangeQuery(n / 4, 3 * n / 4), (3 * n / 4) - (n / 4) + 1);
    EXPECT_EQ(tree.RangeQuery(1, 1), 1);
    EXPECT_EQ(tree.RangeQuery(n, n), 1);
    EXPECT_EQ(tree.RangeQuery(n + 1, n + 10), 0);
    EXPECT_EQ(tree.RangeQuery(0, 0), 0);
}

TEST(AVLTreeRangeQueryComplexTest, AfterSpecificRotations) {
    AVLTree<int> tree;

    const std::vector<int> values = {10, 20, 30, 40, 50, 25, 15, 5, 28, 45};

    std::set<int> reference_set;
    for (int v : values) {
        tree.Insert(v);
        reference_set.insert(v);
    }

    EXPECT_EQ(tree.RangeQuery(5, 50), reference_set.size());
    EXPECT_EQ(tree.RangeQuery(10, 30), 6);
    EXPECT_EQ(tree.RangeQuery(25, 45), 5);
    EXPECT_EQ(tree.RangeQuery(5, 15), 3);
    EXPECT_EQ(tree.RangeQuery(40, 50), 3);
    EXPECT_EQ(tree.RangeQuery(26, 27), 0);
    EXPECT_EQ(tree.RangeQuery(28, 28), 1);
}

TEST(AVLTreeRangeQueryComplexTest, IntegerLimits) {
    AVLTree<int> tree;
    const int min_int = std::numeric_limits<int>::min();
    const int max_int = std::numeric_limits<int>::max();

    tree.Insert(0);
    tree.Insert(min_int);
    tree.Insert(max_int);
    tree.Insert(1);
    tree.Insert(-1);

    if (min_int < -1) tree.Insert(min_int + 1);
    if (max_int > 1) tree.Insert(max_int - 1);

    size_t expected_total = 5;
    if (min_int < -1) expected_total++;
    if (max_int > 1) expected_total++;

    EXPECT_EQ(tree.RangeQuery(min_int, max_int), expected_total);
    EXPECT_EQ(tree.RangeQuery(min_int, min_int), 1);
    EXPECT_EQ(tree.RangeQuery(max_int, max_int), 1);
    EXPECT_EQ(tree.RangeQuery(min_int, 0), (min_int < -1 ? 4u : 3u));
    EXPECT_EQ(tree.RangeQuery(0, max_int), (max_int > 1 ? 4u : 3u));
    EXPECT_EQ(tree.RangeQuery(min_int + 1, -1), (min_int < -1 ? 2u : 1u));
    EXPECT_EQ(tree.RangeQuery(1, max_int - 1), (max_int > 1 ? 2u : 1u));
}

TEST(AVLTreeRangeQueryComplexTest, FloatingPointPrecision) {
    AVLTree<double> tree;
    tree.Insert(1.0);
    tree.Insert(2.0);
    tree.Insert(3.0);
    tree.Insert(1.000000000000001);
    tree.Insert(2.999999999999999);

    EXPECT_EQ(tree.RangeQuery(1.0, 3.0), 5);
    EXPECT_EQ(tree.RangeQuery(1.0, 1.0), 1);
    EXPECT_EQ(tree.RangeQuery(3.0, 3.0), 1);
    EXPECT_EQ(tree.RangeQuery(1.0000000000000005, 1.0000000000000015), 1);
    EXPECT_EQ(tree.RangeQuery(2.9999999999999985, 2.9999999999999995), 1);
    EXPECT_EQ(tree.RangeQuery(1.0, 2.0), 3);
    EXPECT_EQ(tree.RangeQuery(1.1, 2.9), 1);
}

}  // namespace avl_tree

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = google::FATAL;
    ::testing::InitGoogleTest(&argc, argv);
    google::ShutdownGoogleLogging();
    return RUN_ALL_TESTS();
}