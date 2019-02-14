// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information

#pragma once

#include <cstddef>
#include <cinttypes>
#include <initializer_list>

namespace CryptoNote {
struct CheckpointData {
  uint32_t index;
  const char* blockId;
};

const std::initializer_list<CheckpointData> CHECKPOINTS = {
  {       0, "34f61e2d54906689fc249f0d1aecda614ad3434074009a9bfa350bc6b9ef1ff5"},
  {       2, "7ad1ec5d6aa83e315cff3a7eaf09cd75973bf6526036e55373dd01c484f5ac99"},
  {    5000, "09158af9e50f58778e87e2c47450be18a9fdaf8c853d06523da8824e04ab3f51"},
  {   10000, "52f9a112b1983adc2cb800fd6ca74f6a1ef243caf57f3b599a6633c5c1c8079a"},
  {   15000, "c128cdd4ddb7ee560a575ed49bf4dc4e314ad0cc0174453af7cb36f4c9784fbb"},
  {   20000, "fc51021942b92eaaafc2b8b5a256a1948ac3ba4cd633b12b9b3fb307c81a61c7"},
  {   25000, "c2c366fa500fc449ce1aa0a76e93aa5c0981eb12ade7f6c23d798cbeb85229d4"},
  {   30000, "68faaf4f2a2ca1468ea436158306ca9eb9de3a80f8b594adc07a79534dcee4be"},
  {   35000, "850e13e6d51210146184aa6e313850ec43901e28a172d533d5f5b61dcbef4db0"},
  {   38312, "0e48789ec5504b5035e516bbe1582223c8ba6415617c99fde6e6e6f1a7cda067"},
  {   40000, "3f635ec4e4e246a2c833713c0e4dce650241c78df32125c37d7058c98fd2752e"},
  {   45000, "e1038ba936b8b8dd455282bd57f9da88df92b09051219ac5bf5027d63eb0c350"},
  {   50000, "56c3d0583849c0923ee4a435b192553b0f802816b9ea9595acf04852f1a017ce"},
  
};
}

