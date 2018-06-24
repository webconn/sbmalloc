#pragma once

/**
 * \file sbmalloc.h
 * \brief An implementation of a simple block memory allocator
 *
 * \author Nikita Maslov
 * \date 2018
 * \copyright GNU LGPLv3
 */

#include <stddef.h>

#include "mutex_t.h"

/**
 * Pool state structure
 */
struct sbmalloc_pool {
        /* these members are first in prior to initialize the at compile time */
        size_t block_size; /**< memory pool block size in bytes */
        size_t num_blocks; /**< number of blocks in pool */

        size_t num_initialized; /**< number of initialized blocks, used to
                                     avoid pool memory pre-initialization */

        sbmalloc_mutex_t mtx; /**< a mutex to control access to pool */

        size_t free_blocks; /**< number of free blocks in pool at the moment */

        unsigned char *next_free; /**< a pointer to the next free block */

        unsigned char pool[0]; /**< Flexible array member to allow variable-size
                                 pools at compile time */
};

/**
 * A macro to declare a pool with specific size at compile time.
 * To access declared pool in your code, use this:
 *
 * \code{c}
 *     static SBMALLOC_DECLARE_POOL(mypool, 32, 64);
 *     ...
 *     void *myblock = sbmalloc(&mypool.p);
 * \endcode
 */
#define SBMALLOC_DECLARE_POOL(name, bs, nb) union { \
        struct sbmalloc_pool p; \
        unsigned char __mem[sizeof (struct sbmalloc_pool) + \
                (bs) * (nb)]; \
} name = { .p = { .block_size = (bs), \
                  .num_blocks = (nb) } }


/**
 * Initializes the pool
 * \return 0 if initialization is OK or error code otherwise
 */
int sb_pool_init(struct sbmalloc_pool *p);

/**
 * Destroys the pool.
 * \return 0 if destroy is successful or error code otherwise
 */
int sb_pool_destroy(struct sbmalloc_pool *p);

/**
 * Allocates a block in the pool
 * \return pointer to block if allocation is OK or NULL otherwise
 */
void *sbmalloc(struct sbmalloc_pool *p);

/**
 * Deallocates a block by pointer
 * \return 0 if free is successful or error code otherwise
 */
int sbfree(struct sbmalloc_pool *p, void *ptr);
