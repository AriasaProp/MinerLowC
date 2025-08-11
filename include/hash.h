#ifndef HASH_INCLUDED_
#define HASH_INCLUDED_

#include "common.h"

// global variable
typedef union {
  uint8_t bytes[32];
  uint16_t shorts[16];
  uint32_t integers[8];
  uint64_t longs[4];
} sha256;

#ifndef HASH_IMPLEMENTS
extern void bytesToHex (char*, const uint8_t*, const size_t);
extern void hexToBytes (uint8_t*, const char*, const size_t);
extern void bswap32(uint32_t*);
extern uint32_t rotr32(const uint32_t, const size_t);
extern void flip64bytes(uint8_t*);
#endif //HASH_IMPLEMENTS

#ifndef HASH_SHA256_IMPLEMENTS
extern void hash_sha256(uint8_t *, const char *, const size_t);
#endif //HASH_SHA256_IMPLEMENTS

#endif // HASH_INCLUDED_