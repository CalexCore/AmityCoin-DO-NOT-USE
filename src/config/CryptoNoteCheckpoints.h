// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cstddef>
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
  {   40000, "3f635ec4e4e246a2c833713c0e4dce650241c78df32125c37d7058c98fd2752e"},
  {   45000, "e1038ba936b8b8dd455282bd57f9da88df92b09051219ac5bf5027d63eb0c350"},
  {   50000, "56c3d0583849c0923ee4a435b192553b0f802816b9ea9595acf04852f1a017ce"},
  {   55000, "26d05c9d95e6f0f2473f3613086152dd34f3e3b754f60f08ed22c65e0d4f1899"},
  {   60000, "d4ace341cfd0b113424ff2a0c3465cffd8d51cd601f7f9007b00411163821b99"},
  {   65000, "f0053e8df2a8ada99f307ebe07d75836dc7c047bf2eaf7e9959631a01bcabed4"},
  {   70000, "4e2d467d2130df5f2ff6f3fc7cc3c0d364d150aafcaf789ea99bbdae46786a32"},
  
};
}

