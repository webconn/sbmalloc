#include "sbmalloc.h"
#include "config.h"

#include <assert.h>

#include "mutex.h"

/**
 * Implementation of simple block allocation
 * algorithm based on article written by Ben Kenwright
 * (ISBN 978-1-61208-222-6).
 *
 * It works without memory overhead and without loops
 * in control procedures.
 *
 * The idea is to use free blocks to self-store a linked list
 * of these blocks by storing the index of the next free block.
 * The pointer to the head of the list is stored in the
 * pool's control structure.
 */

/** Converts pointer to block index in pool */
static inline size_t addr_to_index(struct sbmalloc_pool *p, void *addr)
{
        return ((unsigned char *) addr - p->pool) / p->block_size;
}

/** Converts block index to pointer in pool */
static inline void *index_to_addr(struct sbmalloc_pool *p, size_t idx)
{
        return p->pool + idx * p->block_size;
}

/** Checks that pointer belongs to this pool */
static void assert_this_pool(struct sbmalloc_pool *p, void *addr)
{
        assert((void *) p->pool <= addr);
        assert((void *) ((unsigned char *) p->pool +
                p->block_size * p->num_blocks) > addr);

}

/**
 * Expected that block_size and num_blocks fields in
 * the stucture are already filled (by SBMALLOC_DECLARE_POOL by
 * example)
 */
int sb_pool_init(struct sbmalloc_pool *p)
{
        int err;

        err = sbmalloc_mutex_init(&p->mtx);
        if (err != 0) {
                return err;
        }

        p->free_blocks = p->num_blocks;
        p->next_free = p->pool;

        p->num_initialized = 0;

        return 0;
}

int sb_pool_destroy(struct sbmalloc_pool *p)
{
        return sbmalloc_mutex_destroy(&p->mtx);
}

void *sbmalloc(struct sbmalloc_pool *p)
{
        int err;

        err = sbmalloc_mutex_lock(&p->mtx);
        if (err != 0) {
                return NULL;
        }

        /* Initialize the next free block if it was not
         * initialized yet */
        if (p->num_initialized < p->num_blocks) {
                unsigned int *el =
                        (unsigned int *) index_to_addr(p, p->num_initialized);
                *el = ++p->num_initialized;
        }

        void *ret = NULL;

        if (p->free_blocks > 0) {
                /* Use current head of free blocks list */
                ret = (void *) p->next_free;
                --p->free_blocks;

                if (p->free_blocks != 0) {
                        /* Shift the head of free blocks list to the next
                         * available block, index of which is written in
                         * current free block */
                        p->next_free = (unsigned char *)
                                index_to_addr(p, *((size_t *) p->next_free));
                } else {
                        /* No free blocks left, delete head */
                        p->next_free = NULL;
                }
        }

        err = sbmalloc_mutex_unlock(&p->mtx);

        /* error on mutex unlock is a very bad sign */
        assert(err == 0);

        return ret;
}

int sbfree(struct sbmalloc_pool *p, void *ptr)
{
        int err;

        err = sbmalloc_mutex_lock(&p->mtx);
        if (err != 0) {
                return err;
        }

        assert_this_pool(p, ptr);

        if (p->next_free != NULL) {
                /* Append current block to free blocks list head */
                *((size_t *) ptr) = addr_to_index(p, p->next_free);
                p->next_free = (unsigned char *) ptr;
        } else {
                /* Restore the head of free blocks list */
                p->next_free = (unsigned char *) ptr;
        }

        ++p->free_blocks;

        err = sbmalloc_mutex_unlock(&p->mtx);
        /* error on mutex unlock is a very bad sign */
        assert(err == 0);

        return 0;

}
