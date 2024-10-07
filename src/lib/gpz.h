#ifndef GPZ_H_65D53412B6E5B29A
#define GPZ_H_65D53412B6E5B29A

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sha1.h"

typedef struct __attribute__((aligned(16)))
{
    uint8_t buf[64];
    uint32_t hash[5];
    uint64_t msg_len;
} GPZ_SHA1;

void GPZ_sha1_init(GPZ_SHA1 *ctx);

void GPZ_sha1_wipe(GPZ_SHA1 *ctx);

bool GPZ_sha1_update(GPZ_SHA1 *ctx, const uint8_t *src, uint64_t src_len);

void GPZ_sha1_digest(GPZ_SHA1 *ctx, uint8_t digest[20]);

#endif
