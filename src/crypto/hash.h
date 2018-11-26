// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2014-2018, The Monero Project
// Copyright (c) 2014-2018, The Aeon Project
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <stddef.h>

#include <CryptoTypes.h>
#include "generic-ops.h"

// Standard Cryptonight Definitions
#define CN_PAGE_SIZE                    2097152
#define CN_SCRATCHPAD                   2097152
#define CN_ITERATIONS                   1048576

// Standard CryptoNight Lite Definitions
#define CN_LITE_PAGE_SIZE               2097152
#define CN_LITE_SCRATCHPAD              1048576
#define CN_LITE_ITERATIONS              524288

// CryptoNight Soft Shell Definitions
/*!
 * \def CN_SOFT_SHELL_MEMORY
 * The minimum scratchpad size in bytes.
 */
#define CN_SOFT_SHELL_MEMORY            (256 * 1024)

/*!
 * \def CN_SOFT_SHELL_WINDOW
 * Number of blocks to be mined from low to peak memory/iterations. Thus the length of the wave.
 * Notice this induces CN_SOFT_SHELL_WINDOW * 2 * BLOCK_TIME until you completed one wave.
 * Since TRTL uses 30 seconds block time with a window of size 2048 you may want to apply your
 * changes here accordingly.
 */
#define CN_SOFT_SHELL_WINDOW            2048

/*!
 * \def CN_SOFT_SHELL_MULTIPLIER
 * Used to determine the amount of additional memory requirements for the scratchpad size for a
 * specific iteration. The maximum scratchpad size is given by
 * CN_SOFT_SHELL_MEMORY + CN_SOFT_SHELL_WINDOW * CN_SOFT_SHELL_WINDOW / CN_SOFT_SHELL_MULTIPLIER
 */
#define CN_SOFT_SHELL_MULTIPLIER        3 // 0.143

/*!
 * \def CN_SOFT_SHELL_ITER
 * Minimal amount of Keccak iteration on the scratchpad.
 */
#define CN_SOFT_SHELL_ITER              (CN_SOFT_SHELL_MEMORY / 2)

/*!
 * \def CN_SOFT_SHELL_PAD_MULTIPLIER
 * The amount of additional bytes added for each block distance away from the nearest low.
 */
#define CN_SOFT_SHELL_PAD_MULTIPLIER    (CN_SOFT_SHELL_WINDOW / CN_SOFT_SHELL_MULTIPLIER)

#define CN_SOFT_SHELL_ITER_MULTIPLIER   (CN_SOFT_SHELL_PAD_MULTIPLIER / 2)

#if (((CN_SOFT_SHELL_WINDOW * CN_SOFT_SHELL_PAD_MULTIPLIER) + CN_SOFT_SHELL_MEMORY) > CN_PAGE_SIZE)
#error The CryptoNight Soft Shell Parameters you supplied will exceed normal paging operations.
#endif

namespace Crypto {

  extern "C" {
#include "hash-ops.h"
  }

  /*
    Cryptonight hash functions
  */

  inline void cn_fast_hash(const void *data, size_t length, Hash &hash) {
    cn_fast_hash(data, length, reinterpret_cast<char *>(&hash));
  }

  inline Hash cn_fast_hash(const void *data, size_t length) {
    Hash h;
    cn_fast_hash(data, length, reinterpret_cast<char *>(&h));
    return h;
  }

  // Standard CryptoNight
  inline void cn_slow_hash_v0(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 0, 0, 0, CN_PAGE_SIZE, CN_SCRATCHPAD, CN_ITERATIONS);
  }

  inline void cn_slow_hash_v1(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 0, 1, 0, CN_PAGE_SIZE, CN_SCRATCHPAD, CN_ITERATIONS);
  }

  inline void cn_slow_hash_v2(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 0, 2, 0, CN_PAGE_SIZE, CN_SCRATCHPAD, CN_ITERATIONS);
  }

  // Standard CryptoNight Lite
  inline void cn_lite_slow_hash_v0(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 0, 0, CN_LITE_PAGE_SIZE, CN_LITE_SCRATCHPAD, CN_LITE_ITERATIONS);
  }

  inline void cn_lite_slow_hash_v1(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 1, 0, CN_LITE_PAGE_SIZE, CN_LITE_SCRATCHPAD, CN_LITE_ITERATIONS);
  }

  inline void cn_lite_slow_hash_v2(const void *data, size_t length, Hash &hash) {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 2, 0, CN_LITE_PAGE_SIZE, CN_LITE_SCRATCHPAD, CN_LITE_ITERATIONS);
  }
  
  // CryptoNight Soft Shell
  inline  void cn_soft_shell_slow_hash_v0(const void *data, size_t length, Hash &hash, uint32_t height) {
    uint32_t base_offset = (height % CN_SOFT_SHELL_WINDOW);
    int32_t offset = (height % (CN_SOFT_SHELL_WINDOW * 2)) - (base_offset * 2);
    if (offset < 0) {
      offset = base_offset;
    }

    uint32_t scratchpad = CN_SOFT_SHELL_MEMORY + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_PAD_MULTIPLIER);
    uint32_t iterations = CN_SOFT_SHELL_ITER + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_ITER_MULTIPLIER);
    uint32_t pagesize = scratchpad;

    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 0, 0, pagesize, scratchpad, iterations);
  }

  inline void cn_soft_shell_slow_hash_v1(const void *data, size_t length, Hash &hash, uint32_t height) {
    uint32_t base_offset = (height % CN_SOFT_SHELL_WINDOW);
    int32_t offset = (height % (CN_SOFT_SHELL_WINDOW * 2)) - (base_offset * 2);
    if (offset < 0) {
      offset = base_offset;
    }

    uint32_t scratchpad = CN_SOFT_SHELL_MEMORY + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_PAD_MULTIPLIER);
    uint32_t iterations = CN_SOFT_SHELL_ITER + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_ITER_MULTIPLIER);
    uint32_t pagesize = scratchpad;

    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 1, 0, pagesize, scratchpad, iterations);
  }

  inline void cn_soft_shell_slow_hash_v2(const void *data, size_t length, Hash &hash, uint32_t height) {
    uint32_t base_offset = (height % CN_SOFT_SHELL_WINDOW);
    int32_t offset = (height % (CN_SOFT_SHELL_WINDOW * 2)) - (base_offset * 2);
    if (offset < 0) {
      offset = base_offset;
    }

    uint32_t scratchpad = CN_SOFT_SHELL_MEMORY + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_PAD_MULTIPLIER);
    uint32_t iterations = CN_SOFT_SHELL_ITER + (static_cast<uint32_t>(offset) * CN_SOFT_SHELL_ITER_MULTIPLIER);
    uint32_t pagesize = scratchpad;

    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash), 1, 2, 0, pagesize, scratchpad, iterations);
  }

  inline void tree_hash(const Hash *hashes, size_t count, Hash &root_hash) {
    tree_hash(reinterpret_cast<const char (*)[HASH_SIZE]>(hashes), count, reinterpret_cast<char *>(&root_hash));
  }

  inline void tree_branch(const Hash *hashes, size_t count, Hash *branch) {
    tree_branch(reinterpret_cast<const char (*)[HASH_SIZE]>(hashes), count, reinterpret_cast<char (*)[HASH_SIZE]>(branch));
  }

  inline void tree_hash_from_branch(const Hash *branch, size_t depth, const Hash &leaf, const void *path, Hash &root_hash) {
    tree_hash_from_branch(reinterpret_cast<const char (*)[HASH_SIZE]>(branch), depth, reinterpret_cast<const char *>(&leaf), path, reinterpret_cast<char *>(&root_hash));
  }
}

CRYPTO_MAKE_HASHABLE(Hash)
