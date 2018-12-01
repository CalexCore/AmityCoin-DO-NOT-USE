#pragma once

#include <vector>

#include <benchmark/benchmark.h>
#include <crypto/hash.h>

class HashBasedBenchmark : public benchmark::Fixture {
public:
  const std::size_t BlockSize;
  const std::size_t BlockCount;
protected:
  HashBasedBenchmark();
  unsigned char const * data() const;
  Crypto::Hash HashPlaceholder;
};
