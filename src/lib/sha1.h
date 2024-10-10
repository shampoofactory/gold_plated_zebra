#ifndef RSP_EXE_H_7A1C950270709A30
#define RSP_EXE_H_7A1C950270709A30

#include <stdint.h>

void _sha1_transform(uint8_t hash[static 20], const uint8_t *msg, uint64_t n_blocks);

void _sha1_store_len(uint8_t block[64], uint64_t len);

#endif