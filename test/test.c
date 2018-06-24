#include "tinytest.h"
#include <stdlib.h>

#include "sbmalloc.h"

#define arr_length(a) ((int) (sizeof ((a)) / sizeof ((a)[0])))

#define TEST_BLOCK_SIZE 32
#define TEST_NUM_BLOCKS 32
static const int blocks_to_free[] = { 1, 3, 4, 10, 15 };

static SBMALLOC_DECLARE_POOL(testpool, TEST_BLOCK_SIZE, TEST_NUM_BLOCKS);

void test_init(void)
{
        ASSERT("size of test pool is correct",
                sizeof (testpool) == sizeof (struct sbmalloc_pool) + 32*32);

        ASSERT("init runs correctly", sb_pool_init(&testpool.p) == 0);

        ASSERT("destroy is OK", sb_pool_destroy(&testpool.p) == 0);
}

void test_alloc(void)
{
        void *p;

        ASSERT("init runs correctly", sb_pool_init(&testpool.p) == 0);

        p = sbmalloc(&testpool.p);
        ASSERT("pointer is not NULL", p != NULL);

        ASSERT("free is OK", sbfree(&testpool.p, p) == 0);

        ASSERT("destroy is OK", sb_pool_destroy(&testpool.p) == 0);
}

void test_fill(void)
{
        void *ps[TEST_NUM_BLOCKS];
        void *p;

        ASSERT("init runs correctly", sb_pool_init(&testpool.p) == 0);

        /* try to allocate all available blocks */
        for (int i = 0; i < arr_length(ps); i++) {
                ps[i] = sbmalloc(&testpool.p);
                ASSERT("pointer is not NULL", ps[i] != NULL);

                /* check that pointers are unique */
                for (int j = 0; j < i; j++) {
                        ASSERT("pointers are unique", ps[i] != ps[j]);
                }
        }

        /* check that next block is unallocatable */
        p = sbmalloc(&testpool.p);
        ASSERT("pointer is NULL", p == NULL);

        /* free some random blocks */
        for (int i = 0; i < arr_length(blocks_to_free); i++) {
                ASSERT("free is OK", sbfree(&testpool.p,
                                ps[blocks_to_free[i]]) == 0);
        }

        /* allocate this number of blocks back */
        for (int i = 0; i < arr_length(blocks_to_free); i++) {
                ps[blocks_to_free[i]] = sbmalloc(&testpool.p);
                ASSERT("pointer is not NULL", ps[blocks_to_free[i]] != NULL);
        }

        /* check that next block is unallocatable */
        p = sbmalloc(&testpool.p);
        ASSERT("pointer is NULL", p == NULL);

        /* check that all allocated blocks here are unique */
        for (int i = 0; i < arr_length(ps); i++) {
               for (int j = 0; j < i; j++) {
                       ASSERT("pointers are unique", ps[i] != ps[j]);
               }
        }

        ASSERT("destroy is OK", sb_pool_destroy(&testpool.p) == 0);
}

int main(int argc, char **argv)
{
        RUN(test_init);
        RUN(test_alloc);
        RUN(test_fill);
        return TEST_REPORT();
}
