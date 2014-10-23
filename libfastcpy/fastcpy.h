/*
 * fastcpy.h
 *
 *  Created on: 22 Oct 2014
 *      Author: up45
 */

#ifndef LIBFASTCPY_FASTCPY_H_
#define LIBFASTCPY_FASTCPY_H_

#include <cstring>
#include "asmlib.h"
#include "dvik_memcpy.h"

inline void *basic_memcpy(void *dest, const void *src, size_t n) {
    return memcpy(dest, src, n);
};

void *duff_memcpy(void *dest, const void *src, size_t n);

static inline void * kernel_memcpy(void * to, const void * from, size_t n)
{
    int d0, d1, d2;

    __asm__ __volatile__(
            "rep ; movsl\n\t"
            "testb $2,%b4\n\t"
            "je 1f\n\t"
            "movsw\n"
            "1:\ttestb $1,%b4\n\t"
            "je 2f\n\t"
            "movsb\n"
            "2:"
            : "=&c" (d0), "=&D" (d1), "=&S" (d2)
              :"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
               : "memory");

    return(to);
}

#define SSE_MMREG_SIZE 16
#define MIN_LEN 0x40  /* 64-byte blocks */
void * sse2_memcpy(void * to, const void * from, size_t len);


#endif /* LIBFASTCPY_FASTCPY_H_ */
