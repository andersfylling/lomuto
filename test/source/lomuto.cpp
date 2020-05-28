#include <doctest/doctest.h>
#include <lomuto/lomuto.h>

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

TEST_CASE("Sorting") {
  std::vector<long> v(1423819);
  std::iota(v.begin(), v.end(), 0);
  do {
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
  } while (std::is_sorted(v.begin(), v.end()));

  CHECK_FALSE(std::is_sorted(v.begin(), v.end()));
  lomuto::sort(v.begin(), v.end());
  CHECK(std::is_sorted(v.begin(), v.end()));
}
