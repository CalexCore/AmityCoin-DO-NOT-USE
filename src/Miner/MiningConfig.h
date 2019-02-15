// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include "version.h"

#include <cstdint>
#include <string>

#include <config/CryptoNoteConfig.h>

namespace CryptoNote {

struct MiningConfig
{
    MiningConfig();

    void parse(int argc, char** argv);

    std::string miningAddress;
    std::string daemonAddress;
    std::string daemonHost;
    int daemonPort;
    size_t threadCount;
    size_t scanPeriod;
    size_t blocksLimit;
    uint64_t firstBlockTimestamp;
    int64_t blockTimestampInterval;
    bool help;
    bool version;
    int donateLevel;  // [0-100] representing percentage of hashpower going to dev fund wallet
    const std::string donateAddress = CryptoNote::DEFAULT_DONATE_ADDRESS;
};

} //namespace CryptoNote
