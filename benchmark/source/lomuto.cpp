#include <lomuto/lomuto.h>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <benchmark/benchmark.h>

constexpr std::size_t SIZE{5000};

template<bool STANDARD>
static void BM_Sorting(benchmark::State& state) {
    if constexpr(STANDARD) {
      state.SetLabel("std::sort");
    } else {
      state.SetLabel("lomuto::sort");
    }
    std::vector<long> v(SIZE);
    std::iota(v.begin(), v.end(), 0);

    for (auto _ : state) {
        state.PauseTiming();
        do {
            std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
        } while (std::is_sorted(v.begin(), v.end()));
        state.ResumeTiming();

        if constexpr(STANDARD) {
          std::sort(v.begin(), v.end());
        } else {
          lomuto::sort(v.begin(), v.end());
        }
    }
}

BENCHMARK_TEMPLATE(BM_Sorting, false);
BENCHMARK_TEMPLATE(BM_Sorting, true);