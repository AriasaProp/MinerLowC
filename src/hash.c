#define HASH_IMPLEMENTS
#include "hash.h"

// hex    ... adb45100724
// byte   ... 00110100111

void bytesToHex(char *out, const uint8_t *in, size_t bit) {
  size_t i, j;
  uint8_t t;
  char *cout = out;
  for (i = 0, j = bit >> 3; i < j; ++i) {
    t = (in[i] >> 4) & 0xf;
    *(cout++) = t + '0' + ((t > 0x9) * ('A' - '0' - 0xa));
    t = in[i] & 0xf;
    *(cout++) = t + '0' + ((t > 0x9) * ('A' - '0' - 0xa));
  }
}

void hexToBytes(uint8_t *out, const char *in, size_t bit) {
  size_t i, j;
  const char *cin = in;
  for (i = 0, j = bit >> 3; i < j; ++i) {
    out[i] = *cin - '0' + ((*cin >= 'A') * ('0' - 'A' + 0xa)) + ((*cin >= 'a') * ('A' - 'a'));
    ++cin;
    out[i] <<= 4;
    out[i] |= *cin - '0' + ((*cin >= 'A') * ('0' - 'A' + 0xa)) + ((*cin >= 'a') * ('A' - 'a'));
    ++cin;
  }
}
// xor swap
// x ^= y, y ^= x, x ^= y
// x -> a^b, y -> b
// x -> a^b, y -> a
// x -> b  , y -> a

// re-arrange 4 byte that flip each byte
void bswap32(uint32_t *pp) {
#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
	*pp = __builtin_bswap32(*pp);
#else
  uint8_t *p = (uint8_t*)pp;
  p[0] ^= p[3], p[1] ^= p[2];
  p[3] ^= p[0], p[2] ^= p[1];
  p[0] ^= p[3], p[1] ^= p[2];
#endif
}
uint32_t rotr32(const uint32_t x, const size_t n) {
#if (defined(__GNUC__) || defined(__clang__)) && __has_builtin(__builtin_rotateright32) // GCC 12+ / Clang 14+
  return __builtin_rotateright32(x, n);
#elif defined(_MSC_VER)
  return _lrotr(x, n);
#else
  return (x >> n) | (x << (32 - n));
#endif
}