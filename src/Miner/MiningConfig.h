// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include "version.h"

#include <cstdint>
#include <string>

namespace CryptoNote {

struct MiningConfig {
  MiningConfig();

  void parse(int argc, char** argv);

  std::string miningAddress;
  std::string daemonHost;
  int daemonPort;
  size_t threadCount;
  size_t scanPeriod;
  uint8_t logLevel;
  size_t blocksLimit;
  uint64_t firstBlockTimestamp;
  int64_t blockTimestampInterval;
  bool help;
  bool version;
  int donateLevel;  // [0-100] representing percentage of hashpower going to dev fund wallet
  const std::string donateAddress = "amitQLHwwcaU5bDjqkZrQ26KHeVdk2mgsC8j8mnBJbpJfi6jMNxUqB42V2ZCn6EJ4z9wK5nVvNBwMVBFubZce3BXAYAJx2YEon";  // address of the dev fund wallet
};

} //namespace CryptoNote
