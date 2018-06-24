Simple fast portable memory block allocator
===========================================

Written in C (C99-compliant) by Maslov Nikita, 2018.
Published under GNU LGPLv3 license.


Description
-----------

A simple and fast portable memory block allocator without memory overhead 
and without loops.

Based on article from Ben Kenwright "Fast Efficient Fixed-Size Memory Pool"
from "Computation Tools 2012" (ISBN 978-1-61208-222-6).

Uses static memory allocated in compile time. Block size and number of
blocks in the pool is configurable with the only limitation that block
must fit one integer to build a free blocks list.

May be used safely in parallel environment (OS, RTOS) by selecting specific
mutex implementation in config.h (currently POSIX pthread mutex is supported).
Another mutex can be added by implementing `sballoc_mutex_*` in mutex.h
and adding a typedef in mutex\_t.h. These functions must return 0 on success
or a non-zero error code (POSIX-compliant).


Build
-----

Can be used as a part of project by including source file and headers.

The code is C99-compliant, so don't forget to add -std=c99 in CFLAGS.


Test
----

The allocator comes with minimal unit tests. Use `make test` to run them.
