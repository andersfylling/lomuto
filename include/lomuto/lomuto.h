#pragma once

#include <range/v3/iterator/concepts.hpp>

#include <range/v3/action/sort.hpp>

namespace lomuto{
  namespace {
/**
Partitions the range [first, last) around a pivot chosen as the minimum of
first[0] and last[-1]. Uses the Lomuto partition algorithm, branch-free.
Returns: a pointer to the new position of the pivot.
*/
    template <ranges::random_access_iterator It>
    It lomuto_partition_branchfree(It begin, It end) {
      assert(begin <= end);
      if (end - begin < 2) {
        return begin; // nothing interesting to do
      }

      // Choose pivot.
      --end;
      if (*begin > *end) {
        std::swap(*begin, *end);
      }
      auto pivot_pos{begin};
      auto pivot{*begin};

      // Prelude.
      do {
        ++begin;
        assert(begin <= end);
      } while (*begin < pivot);

      // Main loop.
      for (auto read{begin + 1}; read < end; ++read) {
        auto x{*read};
        auto less{-int(x < pivot)};
        auto delta{less & (read - begin)};
        begin[delta] = *begin;
        read[-delta] = x;
        begin -= less;
      }

      // Move the pivot to its final slot.
      assert(*begin >= pivot);
      --begin;
      *pivot_pos = *begin;
      *begin = pivot;
      return begin;
    }

/**
Inserts *last into the range to its left, assumed to be sorted and have at
least one element smaller than *last.
*/
    template <ranges::random_access_iterator It>
    void unguarded_linear_insert(It last) {
      auto val = *last;
      --last;
      auto x = *last;
      if (val >= x) {
        return;
      }

      for (;;) {
        last[1] = x;
        --last;
        x = *last;
        if (val >= x) {
          break;
        }
      }

      last[1] = val;
    }

    template <ranges::random_access_iterator It>
    void insertion_sort(It first, It last) {
      assert(first <= last);
      for (auto i{first + 1}; i < last; ++i) {
        auto val = *i;
        if (val < *first) {
          size_t n = i - first - 1;
          do {
            first[n + 1] = first[n];
          } while (n--);
          *first = val;
        } else {
          unguarded_linear_insert(i);
        }
      }
    }
  } // private namespace

  template <std::size_t SORT_THRESHOLD = 16, ranges::random_access_iterator It>
  void sort(It begin, It end) {
    while (std::size_t(std::distance(begin, end)) > SORT_THRESHOLD) {
      auto cut = lomuto_partition_branchfree(begin, end);
      assert(cut >= begin);
      assert(cut < end);
      sort(cut + 1, end);
      end = cut;
    }
    insertion_sort(begin, end);
  }
}