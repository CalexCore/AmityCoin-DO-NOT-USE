#include <benchmark/benchmark.h>

#include "HashBasedBenchmark.h"
#include "crypto/balancedsoftshell.h"

using HashAlgorithm = BalancedSoftShell::cn_balanced_soft_shell_v0_0;

static void HeightArguments(benchmark::internal::Benchmark* b) {
  for (uint32_t i = 0; i <= 2 * HashAlgorithm::WindowSize(); i += 16)
      b->Args({i});
}

BENCHMARK_DEFINE_F(HashBasedBenchmark, BM_CN_BlancedSoftShell)(benchmark::State& state) {
  unsigned char const * data = HashBasedBenchmark::data();
  uint32_t height = static_cast<uint32_t>(state.range(0));
  for (auto _ : state)
  {
    for(std::size_t i = 0; i < BlockCount; ++i)
      HashAlgorithm{}(data + i * BlockSize, BlockSize, HashPlaceholder, height);
  }
}

BENCHMARK_REGISTER_F(HashBasedBenchmark, BM_CN_BlancedSoftShell)->Apply(HeightArguments)->Unit(benchmark::kMillisecond);
