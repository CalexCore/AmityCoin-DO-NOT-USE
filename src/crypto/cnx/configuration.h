// Copyright (c) 2018, The NERVA Developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Galaxia Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <cinttypes>

namespace Crypto {
namespace CNX {

template
<
  uint32_t _WindowSize,
  uint32_t _MinRandomizerSize,
  uint32_t _MaxRandomizerSize,
  uint32_t _MinScratchpadSize,
  uint32_t _MaxScratchpadSize
>
struct Configuration {
  static constexpr int variant() { return 1; }
  static constexpr uint32_t windowSize() { return _WindowSize; }

  static constexpr uint32_t minRandomizerSize() { return _MinRandomizerSize; }
  static constexpr uint32_t maxRandomizerSize() { return _MaxRandomizerSize; }
  static constexpr uint32_t slopeRandomizerSize() { return (maxRandomizerSize() - minRandomizerSize()) / windowSize(); }
  static constexpr uint32_t randomizerSizeForOffset(uint32_t offset) {
    return maxRandomizerSize() - offset * slopeRandomizerSize();
  }

  static_assert (randomizerSizeForOffset(0) == maxRandomizerSize(), "");
  static_assert (randomizerSizeForOffset(windowSize()) == minRandomizerSize(), "");

  static constexpr uint32_t minScratchpadSize() { return _MinScratchpadSize; }
  static constexpr uint32_t maxScratchpadSize() { return _MaxScratchpadSize; }
  static constexpr uint32_t slopeScratchpadSize() { return (maxScratchpadSize() - minScratchpadSize()) / windowSize(); }
  static constexpr uint32_t scratchpadSizeForOffset(uint32_t offset) {
    return minScratchpadSize() + offset * slopeScratchpadSize();
  }

  static_assert (scratchpadSizeForOffset(0) == minScratchpadSize(), "");
  static_assert (scratchpadSizeForOffset(windowSize()) == maxScratchpadSize(), "");

  static_assert (minScratchpadSize() % maxRandomizerSize() == 0, "Scratchpad and randomizer must align in memory.");
  static_assert (slopeScratchpadSize() % slopeRandomizerSize() == 0, "Scratchpad and randomizer must align in memory.");

  static constexpr uint32_t offsetForHeight(uint32_t height) {
    uint32_t base_offset = (height % windowSize());
    int32_t offset = static_cast<int32_t>(height % (windowSize() * 2)) - static_cast<int32_t>(base_offset * 2);
    if (offset < 0) {
      offset = static_cast<int32_t>(base_offset);
    }
    return static_cast<uint32_t>(offset);
  }

  static_assert (offsetForHeight(0) == 0, "");
  static_assert (offsetForHeight(windowSize()) == windowSize(), "");
  static_assert (offsetForHeight(windowSize() + 1) == windowSize() - 1, "");
  static_assert (offsetForHeight(windowSize() - 1) == windowSize() - 1, "");
  static_assert (offsetForHeight(2 * windowSize()) == 0, "");
};

}
}
