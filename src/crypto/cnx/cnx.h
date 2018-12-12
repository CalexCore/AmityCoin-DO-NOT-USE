// Copyright (c) 2018, The NERVA Developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Galaxia Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include "crypto/hash.h"
#include "crypto/cnx/configuration.h"

namespace Crypto {
namespace CNX {

struct Hash_v0
    : Configuration
      <
        2048,
        2 * 1024,
        4 * 1024,
        1 * 256 * 1024,
        2 * 256 * 1024
      >
{
  void operator()(const void *data, size_t length, Hash &hash, uint32_t height) const;
};

}
}
