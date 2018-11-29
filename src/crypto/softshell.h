// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <cinttypes>
#include <array>
#include <limits>

#include "crypto/crypto.h"
#include "crypto/hash.h"

namespace SoftShell {

static inline constexpr uint32_t operator "" _kB(unsigned long long int count){
   return static_cast<uint32_t>(count * 1024);
}

static inline constexpr uint32_t operator "" _MB(unsigned long long int count){
   return static_cast<uint32_t>(count * 1024 * 1024);
}

static inline constexpr uint32_t CryptonightPageSize() { return 2_MB; }
static inline constexpr uint32_t CryptonightScratchpad() { return 2_MB; }
static inline constexpr uint32_t CryptonightIterations() { return 1_MB; }

static inline constexpr uint32_t CryptonightLitePageSize() { return 2_MB; }
static inline constexpr uint32_t CryptonightLiteScratchpad() { return 1_MB; }
static inline constexpr uint32_t CryptonightLiteIterations() { return 512_kB; }

static inline constexpr uint32_t AESBlockSize() { return 16; }
static inline constexpr uint32_t AESKeySize() { return 32; }

static inline constexpr uint32_t CryptonighInitializationBulkSize() { return 8; }
static inline constexpr uint32_t CryptonighInitializationSize() {
  return CryptonighInitializationBulkSize() * AESBlockSize();
}

namespace V0 {

template
<
  uint32_t _WindowSize,
  uint32_t _MinScratchpadSize, uint32_t _MaxScratchpadSize
>
struct Hash {
static inline constexpr uint32_t WindowSize() { return _WindowSize; }
static inline constexpr uint32_t MinScratchpadSize() { return _MinScratchpadSize; }
static inline constexpr uint32_t MaxScratchpadSize() { return _MaxScratchpadSize; }

static_assert (MinScratchpadSize() % 2 == 0, "");
static_assert (MaxScratchpadSize() % 2 == 0, "");

static inline constexpr uint32_t MinIterations() { return MinScratchpadSize() / 2; }
static inline constexpr uint32_t MaxIterations() { return MaxScratchpadSize() / 2; }

static_assert (WindowSize() % 2 == 0, "");
static_assert (MinIterations() <= MaxIterations(), "");
static_assert (MinScratchpadSize() <= MaxScratchpadSize(), "");
static_assert (MinScratchpadSize() % CryptonighInitializationSize() == 0, "");
static_assert (MaxScratchpadSize() % CryptonighInitializationSize() == 0, "");
static_assert (MinScratchpadSize() % AESBlockSize() == 0, "");
static_assert (MaxScratchpadSize() % AESBlockSize() == 0, "");

static inline constexpr uint32_t IterationsIncrementationStep() {
  return (MaxIterations() - MinIterations()) / WindowSize();
}

static_assert ((MaxIterations() - MinIterations()) % WindowSize() == 0, "");
static_assert (MinIterations() + WindowSize() * IterationsIncrementationStep() == MaxIterations(), "");

static inline constexpr uint32_t ScratchpadIncrementationStep() {
  return (MaxScratchpadSize() - MinScratchpadSize()) / WindowSize();
}

static_assert (MinScratchpadSize() + WindowSize() * ScratchpadIncrementationStep() == MaxScratchpadSize(), "");
static_assert ((MaxScratchpadSize() - MinScratchpadSize()) % WindowSize() == 0, "");
static_assert (ScratchpadIncrementationStep() % AESBlockSize() == 0, "");
static_assert (ScratchpadIncrementationStep() % CryptonighInitializationSize() == 0, "");

static inline constexpr uint32_t offsetForHeight(uint32_t height) {
  uint32_t base_offset = (height % WindowSize());
  int32_t offset = (height % (WindowSize() * 2)) - (base_offset * 2);
  if (offset < 0) {
    return base_offset;
  } else {
    return static_cast<uint32_t>(offset);
  }
}

static inline constexpr uint32_t iterationsForOffset(uint32_t offset) {
  return MinIterations() + offset * IterationsIncrementationStep();
}

static inline constexpr uint32_t scratchpadSizeForOffset(uint32_t offset) {
  return MinScratchpadSize() + offset * ScratchpadIncrementationStep();
}

void operator()(const void *data, size_t length, Crypto::Hash& hash, uint32_t height) {
  char* hashPtr = reinterpret_cast<char*>(&hash);
  const uint32_t offset = offsetForHeight(height);
  const uint32_t scratchpad = scratchpadSizeForOffset(offset);
  const uint32_t iterations = iterationsForOffset(offset);
  ::Crypto::cn_slow_hash(data, length, hashPtr, 1, 1, 0, scratchpad, scratchpad, iterations);
}
};

}

using cn_soft_shell_slow_hash_v1 = V0::Hash
<
  1024,
  256_kB, 256_kB * 7
>;

}
