#include "test.h"

int main(int argc, char **argv)
{
    if (CUE_SUCCESS == CU_initialize_registry())
    {
        if (test_sha1())
        {
            CU_basic_set_mode(CU_BRM_VERBOSE);
            CU_basic_run_tests();
        }
        else
        {
            printf("Error: %d", CU_get_error());
        }
        CU_cleanup_registry();
    }
    return CU_get_error();
}