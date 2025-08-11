#define HASH_IMPLEMENTS
#include "hash.h"

// hex    ad b4 51 00 72 4f ...
// byte   ... 00110100111

//  01 02 03 04 05
void hexToBytes(uint8_t *out, const char *in, const size_t bit) {
  const char *cin = in;
  for (uint8_t *bout = out, *bend = out + (bit >> 3); bout < bend; ++bout) {
    *bout = *cin - '0' + ((*cin >= 'A') * ('0' - 'A' + 0xa)) + ((*cin >= 'a') * ('A' - 'a'));
    ++cin;
    *bout <<= 4;
    *bout |= *cin - '0' + ((*cin >= 'A') * ('0' - 'A' + 0xa)) + ((*cin >= 'a') * ('A' - 'a'));
    ++cin;
  }
}
void bytesToHex(char *out, const uint8_t *in, const size_t bit) {
  uint8_t t;
  char *cout = out;
  for (const uint8_t *bin = in, *bend = in + (bit >> 3); bin < bend; ++bin) {
    t = *bin >> 4;
    *cout = t + '0' + ((t > 0x9) * ('A' - '0' - 0xa));
    ++cout;
    t = *bin & 0xf;
    *cout = t + '0' + ((t > 0x9) * ('A' - '0' - 0xa));
    ++cout;
  }
}

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
void flip64bytes(uint8_t *p) {
  for (int i = 0; i < 16; ++i) {
    p[i] ^= p[31 - i];
    p[31 - i] ^= p[i];
    p[i] ^= p[31 - i];
  }
}