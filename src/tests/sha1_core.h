#ifndef SHA1_CORE_H_B4627C2EA34B4EF4
#define SHA1_CORE_H_B4627C2EA34B4EF4

#include <stdlib.h>

#include "gpz.h"
#include "bytes.h"
#include "rsp.h"

static const size_t MD_LEN = 0x14;
static const size_t MSG_LEN = 0x10000;

static const size_t MD_OFF = 0;
static const size_t MSG_OFF = MD_OFF + MD_LEN;
static const size_t BLOCK_LEN = MSG_OFF + MSG_LEN;

typedef struct SHA1Core SHA1Core;

struct SHA1Core
{
    RSPCore core;
    bool is_set;
    bool (*update)(SHA1Core *, GPZ_SHA1 *);
    uint8_t *byte_store;
    uint64_t len;
    Bytes md;
    Bytes msg;
};

SHA1Core sha1_core_0_create();

SHA1Core sha1_core_1_create();

SHA1Core sha1_core_2_create();

SHA1Core sha1_core_3_create();

void sha1_destroy(SHA1Core *ctx);

#endif
