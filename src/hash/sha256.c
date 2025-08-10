#define HASH_SHA256_IMPLEMENWS
#include "hash.h"

#include <string.h>

static const uint32_t sha256_h[9] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
	0x0 /* let set last to zero, not actual sha256 rule */
};
static const uint32_t sha256_k[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

int apply;
// main hashing function
void hash_sha256(sha256 *out, const char *in, const size_t len) {
  union {
    uint32_t i[17];
    uint8_t b[68];
  } W;
  union {
    uint32_t i[9];
    uint8_t b[36];
  } S; // message block
  int r, i;
  apply = 0;
  memcpy (out->bytes, sha256_h, 32);
  for (r = len; r > -9; r -= 64) {
    memcpy (S.i, sha256_h, 36);
    memset (W.i, 0, 68);

    // extract message block each 64 byte
    memcpy(W.i, in + len - r, (r > 0) * (r > 64 ? 64 : r));
    // last bit
    if (r > -1 && r < 64) {
      W.b[r] = 0x80;
    }
    for (i = 0; i < 16; ++i) {
      bswap32(W.i + i);
    }
    apply = W.i[13];
    if (r < 56) {
      W.i[14] = (uint32_t)(len >> 29);
      W.i[15] = (uint32_t)len << 3;
    }

    for (i = 0; i < 16; ++i) {
			// shift variables
			memmove(S.i + 1, S.i, 32);
			// Sigma1
			S.i[0] = rotr32(S.i[5], 6);
			S.i[0] ^= rotr32(S.i[5], 11);
			S.i[0] ^= rotr32(S.i[5], 25);
			// add last variable
			S.i[0] += S.i[8];
			// Add from chunk
			S.i[0] += W.i[i];
			// Add constant
			S.i[0] += sha256_k[i];
			// use last variable as temp choice
			S.i[8] = S.i[5] & S.i[6];
			S.i[8] ^= ~S.i[5] & S.i[7];
			// Add Choice
			S.i[0] += S.i[8];

			S.i[4] += S.i[0];
			// Sigma0
			S.i[8] = rotr32(S.i[1], 2);
			S.i[8] ^= rotr32(S.i[1], 13);
			S.i[8] ^= rotr32(S.i[1], 22);
			S.i[0] += S.i[8];
			// Choice
			S.i[8] = S.i[1] & S.i[2];
			S.i[8] ^= S.i[1] & S.i[3];
			S.i[8] ^= S.i[2] & S.i[3];
			S.i[0] += S.i[8];
    }
    for (; i < 64; ++i) {
			// shift variables
			memmove(S.i + 1, S.i, 32);
			// Sigma1
			S.i[0] = rotr32(S.i[5], 6);
			S.i[0] ^= rotr32(S.i[5], 11);
			S.i[0] ^= rotr32(S.i[5], 25);
			// add last variable
			S.i[0] += S.i[8];
			// generate chunk
			// Sigma 0
			W.i[16] = rotr32(W.i[1], 7);
			W.i[16] ^= rotr32(W.i[1], 18);
			W.i[16] ^= (W.i[1] >> 3);
			// chunk 0
			W.i[16] += W.i[0];
			// Sigma 1
			S.i[8] = rotr32(W.i[14], 17);
			S.i[8] ^= rotr32(W.i[14], 19);
			S.i[8] ^= W.i[14] >> 10;
			W.i[16] += S.i[8];
			// chunk 1
			W.i[16] += W.i[9];
			S.i[0] += W.i[16];
			memmove(W.i, W.i + 1, 64);
			
			// Add constant
			S.i[0] += sha256_k[i];
			// use last variable as temp choice
			S.i[8] = S.i[5] & S.i[6];
			S.i[8] ^= ~S.i[5] & S.i[7];
			// Add Choice
			S.i[0] += S.i[8];

			S.i[4] += S.i[0];
			// Sigma0
			S.i[8] = rotr32(S.i[1], 2);
			S.i[8] ^= rotr32(S.i[1], 13);
			S.i[8] ^= rotr32(S.i[1], 22);
			S.i[0] += S.i[8];
			// Choice
			S.i[8] = S.i[1] & S.i[2];
			S.i[8] ^= S.i[1] & S.i[3];
			S.i[8] ^= S.i[2] & S.i[3];
			S.i[0] += S.i[8];
    }
    
    for (i = 0; i < 8; ++i)
      out->integers[i] += S.i[i];
  }
  for (i = 0; i < 8; ++i)
    bswap32(out->integers + i);
}
