#include "map.hpp"
#include <string>
#include <vector>
#include "allocator.hpp"
#include "gtest/gtest.h"

// тест на вставку элементов
TEST(TestUtils, insert_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i) {
        auto [it, success] = m.insert({ i, values[i] });
        if (!success)
            FAIL() << "Insertion failed";
    }

    // ключ 2 уже есть значит вставки не будет 
    auto [it, success] = m.insert({ 2, "elderberry" });
    if (success)
        FAIL() << "Insertion succeeded but was expected to fail";

    std::vector<mymap::value_type> target = { {0, "apple"}, {1, "banana"}, {2, "cherry"}, {3, "date"} };
    std::vector<mymap::value_type> result;
    for (auto val : m)
        result.push_back(val);
    ASSERT_EQ(result, target);
}

// тест двунаправленного итератора
TEST(TestUtils, iterators_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });
	// прямое итерирование
    ASSERT_EQ(m.begin()->second, "apple");
	// обратное итерирование
    ASSERT_EQ(m.rbegin()->second, "date");

    std::vector<mymap::value_type> target = { {3, "date"}, {2, "cherry"}, {1, "banana"}, {0, "apple"} };
    std::vector<mymap::value_type> result;
    for (auto it = m.rbegin(); it != m.rend(); ++it)
        result.push_back(*it);
    ASSERT_EQ(result, target);
}

// тест на удаление
TEST(TestUtils, erase_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });

    size_t success_count = m.erase(10);
    ASSERT_EQ(success_count, 0) << "Key 10 should not exist";

    success_count = m.erase(2);
    ASSERT_EQ(success_count, 1) << "Key 2 should exist";

    success_count = m.erase(2);
    ASSERT_EQ(success_count, 0) << "Key 2 should not exist anymore";

    std::vector<mymap::value_type> target = { {0, "apple"}, {1, "banana"}, {3, "date"} };
    std::vector<mymap::value_type> result;
    for (auto val : m)
        result.push_back(val);
    ASSERT_EQ(result, target);
}

// тест на поиск
TEST(TestUtils, find_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });

    mymap::iterator it = m.find(5);
    ASSERT_EQ(it, m.end()) << "Key 5 should not exist";

    it = m.find(1);
    ASSERT_NE(it, m.end()) << "Key 1 should exist";
    ASSERT_EQ(it->second, "banana");
}

// проверка содержимого
TEST(TestUtils, contains_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });

    ASSERT_FALSE(m.contains(10)) << "Key 10 should not exist";
    ASSERT_TRUE(m.contains(2)) << "Key 2 should exist";
}

// тест на пустое
TEST(TestUtils, empty_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    ASSERT_TRUE(m.empty()) << "Map should be empty";

    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });

    ASSERT_FALSE(m.empty()) << "Map should not be empty";
}

// тест размера
TEST(TestUtils, size_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i) {
        auto [it, success] = m.insert({ i, values[i] });
        ASSERT_EQ(m.size(), i + 1) << "Size mismatch after insertion";
    }

    m.erase(0);
    ASSERT_EQ(m.size(), 3) << "Size mismatch after erase";
}

//тест очистки
TEST(TestUtils, clear_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        auto [it, success] = m.insert({ i, values[i] });

    m.clear();
    ASSERT_TRUE(m.empty()) << "Map should be empty after clear";
}

//тест оператора индекса
TEST(TestUtils, operator_index_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        m[i] = values[i];

    std::vector<mymap::value_type> target = { {0, "apple"}, {1, "banana"}, {2, "cherry"}, {3, "date"} };
    std::vector<mymap::value_type> result;
    for (auto val : m)
        result.push_back(val);
    ASSERT_EQ(result, target) << "Operator[] test failed";

    m[1] = "blueberry";
    ASSERT_EQ(m[1], "blueberry") << "Operator[] assignment failed";
}

// тест оператора at
TEST(TestUtils, at_test) {
    using mymap = avl::MyMap<int, std::string>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        m[i] = values[i];

    ASSERT_EQ(m.at(2), "cherry") << "At test failed for existing key";

    try {
        m.at(10);
        FAIL() << "At test for non-existing key should throw std::out_of_range";
    } catch (std::out_of_range&) {
        SUCCEED();
    } catch (...) {
        FAIL() << "At test threw unexpected exception";
    }
}

// тест на сравнение
TEST(TestUtils, compare_test) {
    using mymap = avl::MyMap<int, std::string, std::greater<int>>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        m[i] = values[i];

    std::vector<mymap::value_type> target = { {3, "date"}, {2, "cherry"}, {1, "banana"}, {0, "apple"} };
    std::vector<mymap::value_type> result;
    for (auto el : m)
        result.push_back(el);
    ASSERT_EQ(result, target) << "Compare test failed";
}

// тест аллокатора
TEST(TestUtils, allocator_test) {
    using mymap = avl::MyMap<int, std::string, std::less<int>,
        myalloc::allocator<avl::Node<std::pair<const int, std::string>>>>;
    mymap m;
    std::vector<std::string> values = { "apple", "banana", "cherry", "date" };
    for (int i = 0; i < 4; ++i)
        m[i] = values[i];

    std::vector<mymap::value_type> target = { {0, "apple"}, {1, "banana"}, {2, "cherry"}, {3, "date"} };
    std::vector<mymap::value_type> result;
    for (auto val : m)
        result.push_back(val);
    ASSERT_EQ(result, target) << "Allocator test failed";
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
