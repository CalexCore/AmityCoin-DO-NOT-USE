// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018, The Calex Developers
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
  {   10000, "f393e6cebd0cd2f306920b26a6b2c4ae3fadb28b2399b9af93218bb3e859154a"},
  {   15000, "43c306f93ba75dc7115eb484f840a0da231be83bf61a5ff464d4c0144a33dce4"},
  {   20000, "429fe402effe8e53f72a1d054fe17162abb56fda9127d99ac33a6a2c1e036d3c"}
};
}
