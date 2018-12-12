// Copyright (c) 2018, The NERVA Developers
// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2018, The Galaxia Developers
// Copyright (c) 2018, The Calex Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <cinttypes>
#include <limits>

namespace Crypto {
namespace CNX {

class MersenneTwister
{
    private:

        static const uint32_t N = 624;
        static const uint32_t M = 397;
        static const uint32_t MATRIX_A = 0x9908b0df;
        static const uint32_t UPPER_MASK = 0x80000000;
        static const uint32_t LOWER_MASK = 0x7fffffff;
        static const uint32_t TEMPERING_MASK_B = 0x9d2c5680;
        static const uint32_t TEMPERING_MASK_C = 0xefc60000;

        const uint32_t mag01[2] = { 0x0, MATRIX_A };

        inline uint32_t TEMPERING_SHIFT_U(uint32_t y)
        {
            return (y >> 11);
        }

        inline uint32_t TEMPERING_SHIFT_S(uint32_t y)
        {
            return (y << 7);
        }

        inline uint32_t TEMPERING_SHIFT_T(uint32_t y)
        {
            return (y << 15);
        }

        inline uint32_t TEMPERING_SHIFT_L(uint32_t y)
        {
            return (y >> 18);
        }

        uint32_t mt[N] = { 0 };
        uint16_t mti = 0;

    public:

        MersenneTwister(uint32_t seed)
        {
            set_seed(seed);
        }

        void set_seed(uint32_t seed)
        {
            mt[0] = seed & 0xffffffff;
            for (mti = 1; mti < N; mti++)
                mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;
        }

        uint32_t generate_uint()
        {
            uint32_t y;

            if (mti >= N)
            {
                uint32_t kk;

                for (kk = 0; kk < N - M; kk++)
                {
                    y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                    mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
                }

                for (; kk < N - 1; kk++)
                {
                    y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                    mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
                }

                y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
                mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

                mti = 0;
            }

            y = mt[mti++];
            y ^= TEMPERING_SHIFT_U(y);
            y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
            y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
            y ^= TEMPERING_SHIFT_L(y);

            return y;
        }

        uint32_t next(uint32_t min, uint32_t max)
        {
            uint32_t r = generate_uint();
            double div = (double)(0xffffffff) / (double)(max - min);
            return static_cast<uint32_t>(r / div) + min;
        }

        uint32_t next(uint32_t count)
        {
          return  next(0, count - 1);
        }

        void next_bytes(uint8_t* data, uint32_t length)
        {
            for (uint32_t i = 0; i < length; i++)
                data[i] = (uint8_t)(generate_uint() / (0xffffffff / (uint32_t)0xff));
        }

        void next_bytes(char* data, uint32_t length)
        {
            for (uint32_t i = 0; i < length; i++)
                data[i] = (char)(generate_uint() / (0xffffffff / (uint32_t)0xff));
        }
};

}
}
