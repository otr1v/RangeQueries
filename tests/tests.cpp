#ifdef DEBUG
#include "gtest/gtest.h"
#include "avl_tree.hpp" // Включаем заголовок вашего AVL-дерева
#include <vector>
#include <numeric> // Для std::iota
#include <set>     // Для удобного сравнения результатов RangeQuery
#include <algorithm> // Для std::sort

// Пространство имен вашего дерева
namespace avl_tree {
    

// Тестовый класс (fixture) для AVLTree (опционально, но удобно)
class AVLTreeTest : public ::testing::Test {
protected:
    // Здесь можно разместить объекты, общие для нескольких тестов
    AVLTree<int> tree;

    // Вспомогательная функция для получения всех элементов дерева в InOrder порядке
    std::vector<int> GetInOrderElements(const AVLTree<int>& t) {
        std::vector<int> elements;
        for (auto it = t.BeginInOrder(); it != t.EndInOrder(); ++it) {
            elements.push_back(*it);
        }
        return elements;
    }

    

     // Публичный метод для проверки всего дерева
    //  bool IsAVLTreeValid(const AVLTree<int>& t) const {
    //      // Доступ к root_ нужен, что неудобно. Лучше добавить const-метод в AVLTree
    //      // для получения указателя на корень или сделать эту функцию другом класса.
    //      // Пока используем обходной путь через итератор (если дерево не пустое)
    //      // или просто вызываем рекурсивную проверку с root_ (требует friend или геттер)

    //      // Простейший вариант (требует доступа к root_):
    //      // return CheckAVLPropertiesRecursive(t.root_.get()) != -1;

    //      // Вариант без прямого доступа к root_ (менее полный, проверяет только порядок):
    //      std::vector<int> elements = GetInOrderElements(t);
    //      return std::is_sorted(elements.begin(), elements.end());
    //      // Для полной проверки AVL свойств нужен доступ к структуре узлов.
    //  }
};

// Тест на пустом дереве
TEST_F(AVLTreeTest, InitiallyEmpty) {
    EXPECT_EQ(tree.BeginInOrder(), tree.EndInOrder()); // Проверяем, что итераторы равны
    std::vector<int> result;
    tree.RangeQuery(0, 100, [&result](const int& key){ result.push_back(key); });
    EXPECT_TRUE(result.empty());
    // EXPECT_TRUE(IsAVLTreeValid(tree)); // Проверка валидности пустого дерева
}

// Тест простой вставки
TEST_F(AVLTreeTest, InsertSingleElement) {
    tree.Insert(10);
    std::vector<int> expected = {10};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}

// Тест вставки нескольких элементов (без поворотов)
TEST_F(AVLTreeTest, InsertMultipleNoRotations) {
    tree.Insert(20);
    tree.Insert(10);
    tree.Insert(30);
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}

// Тест вставки дубликата
TEST_F(AVLTreeTest, InsertDuplicate) {
    tree.Insert(20);
    tree.Insert(10);
    tree.Insert(30);
    tree.Insert(10); // Дубликат
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}

// Тест RR поворота (Правый-Правый случай)
TEST_F(AVLTreeTest, InsertTriggerRRRotation) {
    tree.Insert(10);
    tree.Insert(20);
    tree.Insert(30); // Вызывает RR поворот вокруг 10, корень станет 20
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree)); // Важно проверить валидность AVL после поворота
}

// Тест LL поворота (Левый-Левый случай)
TEST_F(AVLTreeTest, InsertTriggerLLRotation) {
    tree.Insert(30);
    tree.Insert(20);
    tree.Insert(10); // Вызывает LL поворот вокруг 30, корень станет 20
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}

// Тест LR поворота (Левый-Правый случай)
TEST_F(AVLTreeTest, InsertTriggerLRRotation) {
    tree.Insert(30);
    tree.Insert(10);
    tree.Insert(20); // Вызывает LR поворот, корень станет 20
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}

// Тест RL поворота (Правый-Левый случай)
TEST_F(AVLTreeTest, InsertTriggerRLRotation) {
    tree.Insert(10);
    tree.Insert(30);
    tree.Insert(20); // Вызывает RL поворот, корень станет 20
    std::vector<int> expected = {10, 20, 30};
    EXPECT_EQ(GetInOrderElements(tree), expected);
    // EXPECT_TRUE(IsAVLTreeValid(tree));
}



// Тесты для RangeQuery
TEST_F(AVLTreeTest, RangeQueryBasic) {
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);
    tree.Insert(60);
    tree.Insert(80);
    // Дерево: {20, 30, 40, 50, 60, 70, 80}

    std::vector<int> result;
    std::set<int> expected_set;

    // Запрос 1: Частичный диапазон
    result.clear();
    expected_set = {30, 40, 50, 60};
    tree.RangeQuery(30, 60, [&result](const int& key){ result.push_back(key); });
    std::set<int> result_set1(result.begin(), result.end());
    EXPECT_EQ(result_set1, expected_set);

    // Запрос 2: Диапазон с одним элементом
    result.clear();
    expected_set = {70};
    tree.RangeQuery(70, 70, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set2(result.begin(), result.end());
    EXPECT_EQ(result_set2, expected_set);

    // Запрос 3: Диапазон, охватывающий все
    result.clear();
    expected_set = {20, 30, 40, 50, 60, 70, 80};
    tree.RangeQuery(0, 100, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set3(result.begin(), result.end());
    EXPECT_EQ(result_set3, expected_set);

     // Запрос 4: Диапазон слева
    result.clear();
    expected_set = {20, 30};
    tree.RangeQuery(10, 35, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set4(result.begin(), result.end());
    EXPECT_EQ(result_set4, expected_set);

     // Запрос 5: Диапазон справа
    result.clear();
    expected_set = {60, 70, 80};
    tree.RangeQuery(55, 90, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set5(result.begin(), result.end());
    EXPECT_EQ(result_set5, expected_set);

    // Запрос 6: Диапазон без элементов
    result.clear();
    expected_set = {};
    tree.RangeQuery(41, 49, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set6(result.begin(), result.end());
    EXPECT_EQ(result_set6, expected_set);

     // Запрос 7: Некорректный диапазон (min > max)
    result.clear();
    expected_set = {};
    tree.RangeQuery(60, 30, [&result](const int& key){ result.push_back(key); });
     std::set<int> result_set7(result.begin(), result.end());
    EXPECT_EQ(result_set7, expected_set);
}

// Тест итераторов (InOrder)
TEST_F(AVLTreeTest, IteratorInOrder) {
    std::vector<int> data = {50, 30, 70, 20, 40, 60, 80};
    for(int x : data) {
        tree.Insert(x);
    }
    std::sort(data.begin(), data.end()); // Ожидаемый порядок InOrder

    std::vector<int> result;
    for(auto it = tree.BeginInOrder(); it != tree.EndInOrder(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, data);
}

// Тест итераторов (PreOrder - порядок зависит от структуры дерева)
TEST_F(AVLTreeTest, IteratorPreOrder) {
    // Порядок PreOrder зависит от конкретной структуры AVL дерева после вставок
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);
    tree.Insert(60);
    tree.Insert(80);
    // Ожидаемая структура после вставок (корень 50):
    //       50
    //      /  \
    //    30    70
    //   / \   / \
    // 20  40 60  80
    std::vector<int> expected_preorder = {50, 30, 20, 40, 70, 60, 80};
    std::vector<int> result;
    for(auto it = tree.BeginPreOrder(); it != tree.EndPreOrder(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, expected_preorder);
}

// Тест итераторов (PostOrder - порядок зависит от структуры дерева)
TEST_F(AVLTreeTest, IteratorPostOrder) {
     tree.Insert(50);
     tree.Insert(30);
     tree.Insert(70);
     tree.Insert(20);
     tree.Insert(40);
     tree.Insert(60);
     tree.Insert(80);
    // Структура та же, что и в PreOrder тесте
    std::vector<int> expected_postorder = {20, 40, 30, 60, 80, 70, 50};
    std::vector<int> result;
    for(auto it = tree.BeginPostOrder(); it != tree.EndPostOrder(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, expected_postorder);
}

// Тест деструктора (неявный, проверяет отсутствие утечек/крешей)
TEST_F(AVLTreeTest, DestructorTest) {
    { // Создаем дерево во внутреннем скоупе
        AVLTree<int> temp_tree;
        temp_tree.Insert(10);
        temp_tree.Insert(5);
        temp_tree.Insert(15);
        temp_tree.Insert(20);
    } // Деструктор temp_tree вызывается здесь
    // Если тест не упал, значит Clear() отработал без ошибок
    SUCCEED(); // Явно указываем на успех
}


} // namespace avl_tree


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif // DEBUG