#pragma once

/**
 * \file mutex.h
 * \brief Mutex procedures binding file
 *
 * \author Nikita Maslov
 * \date 2018
 * \copyright GPU LGPLv3
 */

#include "config.h"
#include "mutex_t.h"

#if defined(CONFIG_SBMALLOC_MUTEX_NONE)

static inline int sbmalloc_mutex_init(sbmalloc_mutex_t *mtx) { return 0; }
static inline int sbmalloc_mutex_destroy(sbmalloc_mutex_t *mtx) { return 0; }
static inline int sbmalloc_mutex_lock(sbmalloc_mutex_t *mtx) { return 0; }
static inline int sbmalloc_mutex_unlock(sbmalloc_mutex_t *mtx) { return 0; }

#elif defined(CONFIG_SBMALLOC_MUTEX_POSIX)

#include <pthread.h>

static inline int sbmalloc_mutex_init(sbmalloc_mutex_t *mtx)
{
        return pthread_mutex_init((pthread_mutex_t *) mtx, NULL);
}

static inline int sbmalloc_mutex_destroy(sbmalloc_mutex_t *mtx)
{
        return pthread_mutex_destroy((pthread_mutex_t *) mtx);
}

static inline int sbmalloc_mutex_lock(sbmalloc_mutex_t *mtx)
{
        return pthread_mutex_lock((pthread_mutex_t *) mtx);
}

static inline int sbmalloc_mutex_unlock(sbmalloc_mutex_t *mtx)
{
        return pthread_mutex_unlock((sbmalloc_mutex_t *) mtx);
}

#endif
