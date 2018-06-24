#pragma once

/**
 * \file mutex_t.h
 * \brief Mutex typedefs
 */

#include "config.h"

#if defined(CONFIG_SBMALLOC_MUTEX_NONE)

typedef unsigned char sbmalloc_mutex_t;

#elif defined(CONFIG_SBMALLOC_MUTEX_POSIX)

#include <pthread.h>

typedef pthread_mutex_t sbmalloc_mutex_t;

#else

#error "No mutex implementation selected in configuration!"

#endif
