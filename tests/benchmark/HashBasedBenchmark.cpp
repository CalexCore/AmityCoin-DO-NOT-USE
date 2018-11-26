#include "HashBasedBenchmark.h"

#include <memory>
#include <climits>
#include <random>

namespace  {
using random_bytes_engine = std::independent_bits_engine<
  std::default_random_engine, CHAR_BIT, uint16_t>;
}

HashBasedBenchmark::HashBasedBenchmark()
  : BlockSize{76},
    BlockCount{10}
{
  /* */
}

const unsigned char *HashBasedBenchmark::data() const
{
  static std::unique_ptr<std::vector<uint16_t>> __Data;
  if(__Data.get() == nullptr) {
    __Data = std::make_unique<std::vector<uint16_t>>();
    random_bytes_engine rbe;
    __Data->resize(BlockCount * BlockSize / 2);
    std::generate(__Data->begin(), __Data->end(), std::ref(rbe));
  }
  return reinterpret_cast<unsigned char*>(__Data->data());
}
