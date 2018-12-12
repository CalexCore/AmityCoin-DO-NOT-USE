// Copyright (c) 2018, The NERVA Developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Galaxia Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#include "crypto/cnx/randomizer.h"

#include <cmath>

Crypto::CNX::Randomizer::Randomizer(uint32_t maxSize)
{
  Handle.size = 0;

  _Operators.resize(maxSize);
  Handle.operators = &_Operators[0];

  _Indices.resize(maxSize);
  Handle.indices = &_Indices[0];

  _Values.resize(maxSize);
  Handle.values = &_Values[0];
}

Crypto::CNX::Randomizer::~Randomizer()
{
  /* */
}

void Crypto::CNX::Randomizer::reset(uint32_t size)
{
  Handle.size = size;
  Handle.operationsIndex = 0;
  std::memset(_Operators.data(), 0, _Operators.size() * sizeof(uint8_t));
  std::memset(_Indices.data(), 0, _Indices.size() * sizeof(uint32_t));
  std::memset(_Values.data(), 0, _Values.size() * sizeof(int8_t));
}

uint32_t Crypto::CNX::Randomizer::size() const
{
  return Handle.size;
}
