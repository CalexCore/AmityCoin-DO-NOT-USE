#include <benchmark/benchmark.h>

#include "crypto/hash.h"
#include "crypto/balancedsoftshell.h"

#include "HashBasedBenchmark.h"

using Config = BalancedSoftShell::cn_balanced_soft_shell_v0_0;

static void CustomArgs(benchmark::internal::Benchmark* b) {
  const int StepSize = 16;
  for (uint32_t iterOffset = 0; iterOffset <= Config::WindowSize(); iterOffset += StepSize) {
      for (uint32_t pageSizeOffset = 0; pageSizeOffset <= Config::WindowSize(); pageSizeOffset +=StepSize) {
          b->Args({iterOffset, pageSizeOffset});
      }
  }
}

BENCHMARK_DEFINE_F(HashBasedBenchmark, BM_CN_Variations)(benchmark::State& state) {
  unsigned char const * data = HashBasedBenchmark::data();
  uint32_t iterations = Config::iterationsForOffset(static_cast<uint32_t>(state.range(0)));
  uint32_t scratchpad = Config::scratchpadSizeForOffset(static_cast<uint32_t>(state.range(1)));
  for (auto _ : state)
  {
      (void)_;
      for(std::size_t i = 0; i < BlockCount; ++i) {
          Crypto::cn_slow_hash(data + i * BlockSize, BlockSize, reinterpret_cast<char *>(&HashPlaceholder),
                               1, 0, 0, scratchpad, scratchpad, iterations);
      }
  }
  state.counters["Iterations"] = static_cast<uint32_t>(state.range(0));
  state.counters["PageSize"] = static_cast<uint32_t>(state.range(1));
}

BENCHMARK_REGISTER_F(HashBasedBenchmark, BM_CN_Variations)->Apply(CustomArgs)->Unit(benchmark::kMillisecond);
