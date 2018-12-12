// Copyright (c) 2018, The NERVA Developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Galaxia Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#include "crypto/cnx/cnx.h"

#include <vector>
#include <memory>
#include <random>

#include "crypto/keccak.h"

#include "crypto/cnx/randomizer.h"
#include "crypto/cnx/mersenne-twister.h"

namespace {

void (*const extra_hashes[4])(const void *, size_t, char *) = {
    Crypto::hash_extra_blake, Crypto::hash_extra_groestl, Crypto::hash_extra_jh, Crypto::hash_extra_skein
};

/*!
 * \attention Randomizer::size must be a multiple of 32
 * \attention Length must be at least 32 byte.
 */
void init_randomizer(const void *data, size_t length, Crypto::CN_ADAPTIVE_RandomValues* randomizer, Crypto::Hash &hash)
{
  using namespace Crypto;

  static thread_local std::vector<char> __Data(128u);

  std::memset(__Data.data(), 0, 128);
  for(size_t i = 0; i < 4; ++i) {
    extra_hashes[i](data, length, __Data.data() + i * 32);
  }
  length = 128;

  cn_fast_hash(__Data.data(), length, reinterpret_cast<char*>(&hash));

  // We assume the hash has been already initialized with some values from the caller, as well as the seed, for initial run.
  for(uint32_t i = 0; i < randomizer->size / 32; ++i) {
    const uint32_t offset = i * 32;

    // First: Pick some random operatos using keccak1600, note since we are using __Data they depend on the previous iteration
    std::memcpy(randomizer->operators + offset, &hash, 32);
    randomizer->operationsIndex = hash.data[0];

    // Second: Make the values of the randomizer dependent on the last iterations hashPtr, calculated lastly. For the first loop
    // we simply have chosen cn_fast_hash.
    (reinterpret_cast<uint8_t*>(randomizer->values) + offset)[0] = (randomizer->operators + offset)[31];
    (reinterpret_cast<uint8_t*>(randomizer->values) + offset)[1] = (randomizer->operators + offset)[30];
    (reinterpret_cast<uint8_t*>(randomizer->values) + offset)[2] = (randomizer->operators + offset)[29];
    for(uint32_t j = 2; j < 32; ++j) {
      cn_adaptive_apply_operator(
        reinterpret_cast<uint8_t*>(randomizer->values) + offset + j,
        reinterpret_cast<int8_t*>(randomizer->values) + offset + j - 1,
        randomizer->operationsIndex,
        reinterpret_cast<uint8_t*>(randomizer->values) + offset + j - 2, 1);
    }

    // Third: We need to initialize the indices, we will not bother for now whether they encode legit addresses, the cn_adaptive
    // algorithm will take care of this within the implementation itself.
    *(reinterpret_cast<int8_t*>(randomizer->indices + offset) + 0) = *(randomizer->values + offset + 31);
    *(reinterpret_cast<int8_t*>(randomizer->indices + offset) + 1) = *(randomizer->values + offset + 30);
    *(reinterpret_cast<int8_t*>(randomizer->indices + offset) + 2) = *(randomizer->values + offset + 29);
    for(uint32_t j = 2; j < 32 * sizeof(uint32_t); ++j) {
      cn_adaptive_apply_operator(
        reinterpret_cast<uint8_t*>(randomizer->indices + offset) + j,
        reinterpret_cast<int8_t*>(randomizer->indices + offset) + j - 1,
        randomizer->operationsIndex,
        reinterpret_cast<uint8_t*>(randomizer->indices + offset) + j - 2, 1);
    }

    // Fourth we vary our data block using the initialized part of the randomizer
    for(uint16_t j = 0; j < length; ++j) {
        const uint32_t rI = offset + (j % 32);
        cn_adaptive_apply_operator(
              reinterpret_cast<uint8_t*>(__Data.data()) + j,
              randomizer->values + offset + (randomizer->indices[rI] % 32),
              randomizer->operationsIndex,
              randomizer->operators + rI, 1);
    }

     // Lastly: In order to make every iteration dependent on the previous we will update our hash to yield the new operators
     // for the next iteration.
    extra_hashes[static_cast<uint8_t>(__Data[128 - 1]) % 4](__Data.data(), 128, reinterpret_cast<char*>(&hash));
  }
}

void init_salt(char* salt, size_t saltLength, Crypto::Hash &hash, Crypto::CN_ADAPTIVE_RandomValues* randomizer) {
  using namespace Crypto;

  char* hashPtr = reinterpret_cast<char*>(&hash);
  // Make sure we depedent on the last init_randomizer step
  salt[0] = hashPtr[0];
  salt[1] = hashPtr[1];
  salt[2] = hashPtr[2];
  randomizer->operationsIndex = hash.data[3];
  for(size_t i = 2; i < saltLength; ++i) {
    const uint32_t iR = i % randomizer->size;
    salt[i] = randomizer->values[iR];
    // In order to make every byte very depedent to be evaulated in the sequence given we will alter the randomizer using the
    // previous byte itself, somehow like the scratchpad alters itself.
    cn_adaptive_apply_operator(reinterpret_cast<uint8_t*>(salt) + i, reinterpret_cast<int8_t*>(salt) + i - 1,
                               randomizer->operationsIndex, reinterpret_cast<uint8_t*>(salt) + i - 2, 1);
  }
}

}

void Crypto::CNX::Hash_v0::operator()(const void *data, size_t length, Crypto::Hash &hash, uint32_t height) const
{
  static thread_local std::unique_ptr<Randomizer> __Randomizer = std::make_unique<Randomizer>(maxRandomizerSize());
  static thread_local std::unique_ptr<MersenneTwister> __Twister = std::make_unique<MersenneTwister>(0);
  static thread_local std::vector<char> __Salt;

  std::memset(&hash, 0, sizeof (hash));
  cn_fast_hash(data, length, reinterpret_cast<char*>(&hash));

  for(std::size_t i = 0; i < 1; ++i) {
    const uint32_t offset = offsetForHeight(height);
    const uint32_t scratchpadSize = scratchpadSizeForOffset(offset);
    const uint32_t randomizerSize = randomizerSizeForOffset(offset);

    __Randomizer->reset(randomizerSize);
   init_randomizer(data, length, &__Randomizer->Handle, hash);

    // We dont have a wrapper for the salt so we plain
    __Salt.resize(__Randomizer->size(), 0);
    std::memset(__Salt.data(), 0, __Salt.size() * sizeof (char));
    init_salt(__Salt.data(), __Randomizer->size(), hash, &__Randomizer->Handle);
    cn_fast_hash(__Salt.data(), __Randomizer->size(), reinterpret_cast<char*>(&hash));

    const uint32_t seed = (*reinterpret_cast<uint32_t*>(&hash)) ^ height;
    __Twister->set_seed(seed);

    const uint8_t lookup[] { 2, 4, 8 };
    const uint8_t tempLookup[] = {
      lookup[__Twister->generate_uint() % sizeof(lookup)],
      lookup[__Twister->generate_uint() % sizeof(lookup)],
      lookup[__Twister->generate_uint() % sizeof(lookup)],
    };
    const uint8_t tempLookupIndex = seed % sizeof(tempLookup);

    auto xx = static_cast<uint16_t>(seed % __Twister->next(20, 40) + __Twister->next(20, 40));
    auto yy = static_cast<uint16_t>(seed % __Twister->next(20, 40) + __Twister->next(20, 40));
    auto zz = static_cast<uint16_t>(seed % __Twister->next(20, 40) + __Twister->next(20, 40));
    auto ww = static_cast<uint16_t>(seed % __Twister->next(1, 20000));
    cn_adaptive_slow_hash(data, length, reinterpret_cast<char*>(&hash), variant(), false, scratchpadSize / 2,
                          &__Randomizer->Handle, __Salt.data(), tempLookup[tempLookupIndex], xx, yy, zz, ww, scratchpadSize);
  }
}
