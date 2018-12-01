#include <benchmark/benchmark.h>

#include "HashBasedBenchmark.h"
#include "crypto/softshell.h"

using HashFunc = SoftShell::amity_cn_ss_v0_0;
static void HeightArguments(benchmark::internal::Benchmark* b) {
  for (uint32_t i = 0; i <= HashFunc::WindowSize() * 2; i += 1)
      b->Args({i});
}

BENCHMARK_DEFINE_F(HashBasedBenchmark, BM_CN_SoftShell)(benchmark::State& state) {
  unsigned char const * data = HashBasedBenchmark::data();
  uint32_t height = static_cast<uint32_t>(state.range(0));
  for (auto _ : state)
  {
    for(std::size_t i = 0; i < BlockCount; ++i)
      HashFunc{}(data + i * BlockSize, BlockSize, HashPlaceholder, height);
  }
}

BENCHMARK_REGISTER_F(HashBasedBenchmark, BM_CN_SoftShell)->Apply(HeightArguments)->Unit(benchmark::kMillisecond);
