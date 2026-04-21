#ifndef SORTING_H
#define SORTING_H

// Header-only

// Сортировки требуют декрементируемые итераторы.
#include <iterator>
#include <type_traits>
#include <utility>

// ----------------------------- Пузырёк (например) --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, iterator_category>,
                  "bubble_sort requires bidirectional iterators");

    if (begin == end) {
        return;
    }

    Iterator unsorted_end = end;

    while (begin != unsorted_end) {
        Iterator cur = begin;
        Iterator next = begin;
        ++next;

        if (next == unsorted_end) {
            break;
        }

        while (next != unsorted_end) {
            if (*next < *cur) {
                using std::swap;
                swap(*cur, *next);
            }
            ++cur;
            ++next;
        }

        --unsorted_end;
    }
}

// --------------------------- QuickSort (например) ---------------------------------------
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, iterator_category>,
                  "quick_sort requires bidirectional iterators");

    if (begin == end) {
        return;
    }

    Iterator next = begin;
    ++next;

    if (next == end) {
        return;
    }

    Iterator pivot_iterator = end;
    --pivot_iterator;

    const auto pivot = *pivot_iterator;

    Iterator left = begin;

    for (Iterator current = begin; current != pivot_iterator; ++current) {
        if (*current < pivot) {
            if (current != left) {
                using std::swap;
                swap(*current, *left);
            }
            ++left;
        }
    }

    if (left != pivot_iterator) {
        using std::swap;
        swap(*left, *pivot_iterator);
    }

    quick_sort(begin, left);

    Iterator right_begin = left;
    ++right_begin;
    quick_sort(right_begin, end);
}

#endif // SORTING_H
