#pragma once

/**
 * \file config.h
 * \brief Configuration for a simple block memory allocator
 *
 * \author Nikita Maslov
 * \date 2018
 * \copyright GNU LGPLv3
 */


/** Allocatable block size in bytes */
#define CONFIG_SBMALLOC_BLOCK_SIZE 32

/** Size of memory pool in blocks */
#define CONFIG_SBMALLOC_POOL_SIZE 128

/**
 * Mutex implementation
 * Choose mutex implementation depending
 * on platform
 */

/** Don't use mutex at all */
/* #define CONFIG_SBMALLOC_MUTEX_NONE */

/** Use POSIX-compatible mutex (Linux, NuttX etc.) */
#define CONFIG_SBMALLOC_MUTEX_POSIX
