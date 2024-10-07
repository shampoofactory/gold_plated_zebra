#ifndef SHA1_TEST_H_1681ECFE34B702DA
#define SHA1_TEST_H_1681ECFE34B702DA

#include <stdlib.h>

#include "CUnit/Basic.h"

#include "bytes.h"
#include "gpz.h"
#include "rsp.h"
#include "sha1_core.h"

extern bool test_sha1();

#define CAVP "res/cavp/shabytetestvectors/"

#define SHA1_TEST(tag, n, file)                                   \
    static void test_SHA1_##n##_##tag(void)                       \
    {                                                             \
        char *filename = CAVP #file ".rsp";                       \
        RSP rsp = rsp_create(0x100000);                           \
        SHA1Core core = sha1_core_##n##_create();                 \
        CU_ASSERT(rsp_execute(&rsp, (RSPCore *)&core, filename)); \
    }

#endif
