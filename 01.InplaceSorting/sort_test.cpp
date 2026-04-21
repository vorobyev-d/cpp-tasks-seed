#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <list>
#include <vector>

// А вот тут collvalue.h не надо, берите просто инты
#include "sorting.h"

// Но проверьте, что сортировки таки работают...


TEST(BubbleSortTest, SortsEmptyVector) {
    std::vector<int> data;
    bubble_sort(data.begin(), data.end());
    EXPECT_TRUE(data.empty());
}

TEST(BubbleSortTest, SortsSingleElement) {
    std::vector<int> data = {35};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({35}));
}

TEST(BubbleSortTest, SortsAlreadySorted) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(BubbleSortTest, SortsWithDuplicates) {
    std::vector<int> data = {4, 2, 5, 2, 1, 4};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 2, 4, 4, 5}));
}

TEST(BubbleSortTest, SortsNegativeNumbers) {
    std::vector<int> data = {0, -3, 5, -1, 2};
    bubble_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({-3, -1, 0, 2, 5}));
}


TEST(QuickSortTest, SortsEmptyVector) {
    std::vector<int> data;
    quick_sort(data.begin(), data.end());
    EXPECT_TRUE(data.empty());
}

TEST(QuickSortTest, SortsSingleElement) {
    std::vector<int> data = {42};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({42}));
}

TEST(QuickSortTest, SortsAlreadySorted) {
    std::vector<int> data = {1, 2, 3, 4, 5};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(QuickSortTest, SortsWithDuplicates) {
    std::vector<int> data = {4, 2, 5, 2, 1, 4};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({1, 2, 2, 4, 4, 5}));
}

TEST(QuickSortTest, SortsNegativeNumbers) {
    std::vector<int> data = {0, -3, 5, -1, 2};
    quick_sort(data.begin(), data.end());
    EXPECT_EQ(data, std::vector<int>({-3, -1, 0, 2, 5}));
}
