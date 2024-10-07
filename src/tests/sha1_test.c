#include "sha1_test.h"

SHA1_TEST(short, 0, SHA1ShortMsg)
SHA1_TEST(short, 1, SHA1ShortMsg)
SHA1_TEST(short, 2, SHA1ShortMsg)

SHA1_TEST(long, 0, SHA1LongMsg)
SHA1_TEST(long, 1, SHA1LongMsg)
SHA1_TEST(long, 2, SHA1LongMsg)
SHA1_TEST(long, 3, SHA1LongMsg)

static int init_suite(void)
{
    return 0;
}

static int clean_suite(void)
{
    return 0;
}

bool test_sha1()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("SHA1", init_suite, clean_suite);
    return NULL != pSuite &&
           NULL != CU_add_test(pSuite, "SHA1_0_S", test_SHA1_0_short) &&
           NULL != CU_add_test(pSuite, "SHA1_0_L", test_SHA1_0_long) &&
           NULL != CU_add_test(pSuite, "SHA1_1_S", test_SHA1_1_short) &&
           NULL != CU_add_test(pSuite, "SHA1_1_L", test_SHA1_1_long) &&
           NULL != CU_add_test(pSuite, "SHA1_2_S", test_SHA1_2_short) &&
           NULL != CU_add_test(pSuite, "SHA1_2_L", test_SHA1_2_long) &&
           NULL != CU_add_test(pSuite, "SHA1_3_L", test_SHA1_3_long)

        ;
}
