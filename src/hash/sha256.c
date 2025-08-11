#define HASH_SHA256_IMPLEMENTS
#include "hash.h"

#include <string.h>
#include <inttypes.h>

static const uint32_t sha256_h[8] = {
  0x6a09e667,
  0xbb67ae85,
  0x3c6ef372,
  0xa54ff53a,
  0x510e527f,
  0x9b05688c,
  0x1f83d9ab,
  0x5be0cd19
};
static const uint32_t sha256_k[64] = {
  0x428a2f98,
  0x71374491,
  0xb5c0fbcf,
  0xe9b5dba5,
  0x3956c25b,
  0x59f111f1,
  0x923f82a4,
  0xab1c5ed5,
  0xd807aa98,
  0x12835b01,
  0x243185be,
  0x550c7dc3,
  0x72be5d74,
  0x80deb1fe,
  0x9bdc06a7,
  0xc19bf174,
  0xe49b69c1,
  0xefbe4786,
  0x0fc19dc6,
  0x240ca1cc,
  0x2de92c6f,
  0x4a7484aa,
  0x5cb0a9dc,
  0x76f988da,
  0x983e5152,
  0xa831c66d,
  0xb00327c8,
  0xbf597fc7,
  0xc6e00bf3,
  0xd5a79147,
  0x06ca6351,
  0x14292967,
  0x27b70a85,
  0x2e1b2138,
  0x4d2c6dfc,
  0x53380d13,
  0x650a7354,
  0x766a0abb,
  0x81c2c92e,
  0x92722c85,
  0xa2bfe8a1,
  0xa81a664b,
  0xc24b8b70,
  0xc76c51a3,
  0xd192e819,
  0xd6990624,
  0xf40e3585,
  0x106aa070,
  0x19a4c116,
  0x1e376c08,
  0x2748774c,
  0x34b0bcb5,
  0x391c0cb3,
  0x4ed8aa4a,
  0x5b9cca4f,
  0x682e6ff3,
  0x748f82ee,
  0x78a5636f,
  0x84c87814,
  0x8cc70208,
  0x90befffa,
  0xa4506ceb,
  0xbef9a3f7,
  0xc67178f2
};

/* Elementary functions used by SHA256 */
#define Ch(x, y, z)     ((x & (y ^ z)) ^ z)
#define Maj(x, y, z)    ((x & (y | z)) | (y & z))
#define S0(x)           (rotr32(x, 2) ^ rotr32(x, 13) ^ rotr32(x, 22))
#define S1(x)           (rotr32(x, 6) ^ rotr32(x, 11) ^ rotr32(x, 25))
#define s0(x)           (rotr32(x, 7) ^ rotr32(x, 18) ^ (x >> 3))
#define s1(x)           (rotr32(x, 17) ^ rotr32(x, 19) ^ (x >> 10))

/* SHA256 round function */
#define RND(a, b, c, d, e, f, g, h, k) \
do { \
t0 = h + S1(e) + Ch(e, f, g) + k; \
t1 = S0(a) + Maj(a, b, c); \
d += t0; \
h  = t0 + t1; \
} while (0)

/* Adjusted round function for rotating state */
#define RNDr(S, W, i) \
RND(S[(64 - i) % 8], S[(65 - i) % 8], \
S[(66 - i) % 8], S[(67 - i) % 8], \
S[(68 - i) % 8], S[(69 - i) % 8], \
S[(70 - i) % 8], S[(71 - i) % 8], \
W + sha256_k[i])


/*
 * SHA256 block compression function.  The 256-bit state is transformed via
 * the 512-bit input block to produce a new state.
 */
void sha256_transform(uint32_t *state, const uint32_t *block, int swap) {
  uint32_t W[17], S[8], t0, t1;
  int i;

  memcpy(W, block, 64);
  memcpy(S, state, 32);
  for (i = 0; i < 16; ++i) {
    if (swap) bswap32(W + i);
    RNDr(S, W[i], i);
  }
  for (; i < 64; ++i) {
    W[16] = s1(W[14]) + W[9] + s0(W[1]) + W[0];
    RNDr(S, W[16], i);
    memcpy (W, W + 1, 64);
  }
  for (i = 0; i < 8; ++i) {
    state[i] += S[i];
  }
}
static const uint32_t sha256d_hash1[16] = {
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x80000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000100
};


void hash_sha256(uint8_t *hash, const char *data, const size_t len) {
  uint32_t S[16], T[16];
  int i, r;

  memcpy(S, sha256_h, 32);
  for (r = len; r > -9; r -= 64) {
    if (r < 64)
      memset(T, 0, 64);
    memcpy(T, data + len - r, ICLAMP (0, r, 64));
    if (r >= 0 && r < 64)
    ((unsigned char *)T)[r] = 0x80;
    for (i = 0; i < 16; ++i)
      bswap32(T + i);
    if (r < 56)
    T[15] = 8 * len;
    sha256_transform(S, T, 0);
    // transform
  }
  // what is use for?
  /*
  memcpy(S + 8, sha256d_hash1 + 8, 32);
  memcpy(T, sha256_h, 32);
  sha256_transform(T, S, 0);
  */
  memcpy(hash, S, 32);
  for (i = 0; i < 8; ++i) {
    bswap32(((uint32_t *)hash) + i);
  }
}