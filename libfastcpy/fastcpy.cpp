/*
 * fastcpy.cpp
 *
 *  Created on: 22 Oct 2014
 *      Author: up45
 */

#include "fastcpy.h"

void* duff_memcpy(void* dest, const void* src, size_t count)
{
    register char *to=(char*)dest, *from=(char*)src;
    register int n = (count + 7) / 8;      /* count > 0 assumed */
    switch (count % 8)
    {
        case 0:        do {  *to = *from++;
        case 7:              *to = *from++;
        case 6:              *to = *from++;
        case 5:              *to = *from++;
        case 4:              *to = *from++;
        case 3:              *to = *from++;
        case 2:              *to = *from++;
        case 1:              *to = *from++;
    } while (--n > 0);
    }
    return to;
}

/* for small memory blocks (<256 bytes) this version is faster */
#define small_memcpy(to,from,n)\
{\
register unsigned long int dummy;\
__asm__ __volatile__(\
  "rep; movsb"\
  :"=&D"(to), "=&S"(from), "=&c"(dummy)\
  :"0" (to), "1" (from),"2" (n)\
  : "memory");\
}

void * sse2_memcpy(void * to, const void * from, size_t len)
{
    void *retval;
    size_t i;
    retval = to;

    /* PREFETCH has effect even for MOVSB instruction ;) */
    /* Is that useful ? kysucix */

    __asm__ __volatile__ (
            "   prefetchnta (%0)\n"
            "   prefetchnta 64(%0)\n"
            "   prefetchnta 128(%0)\n"
            "   prefetchnta 192(%0)\n"
            "   prefetchnta 256(%0)\n"
            /*
                          "   prefetchnta 320(%0)\n"
                          "   prefetchnta 384(%0)\n"
                          "   prefetchnta 448(%0)\n"
                          "   prefetchnta 512(%0)\n"
             */
            : : "r" (from) );

    if (len >= MIN_LEN)
    {
        register unsigned long int delta;
        /* Align destinition to MMREG_SIZE -boundary */
        delta = ((unsigned long int)to)&(SSE_MMREG_SIZE-1);
        if (delta)
        {
            delta=SSE_MMREG_SIZE-delta;
            len -= delta;
            small_memcpy(to, from, delta);
        }
        i = len >> 7; /* len/128 */
        len&=127;
        if (((unsigned long)from) & 15)
        {
            /* if SRC is misaligned */
            for (; i>0; i--)
            {
                __asm__ __volatile__ (
                        "prefetchnta 640(%0)\n"

                        "movdqu (%0), %%xmm0\n"
                        "movdqu 16(%0), %%xmm1\n"
                        "movdqu 32(%0), %%xmm2\n"
                        "movdqu 48(%0), %%xmm3\n"

                        "movntdq %%xmm0, (%1)\n"
                        "movntdq %%xmm1, 16(%1)\n"
                        "movntdq %%xmm2, 32(%1)\n"
                        "movntdq %%xmm3, 48(%1)\n"

                        "movdqu 64(%0), %%xmm4\n"
                        "movdqu 80(%0), %%xmm5\n"
                        "movdqu 96(%0), %%xmm6\n"
                        "movdqu 112(%0), %%xmm7\n"

                        "movntdq %%xmm4, 64(%1)\n"
                        "movntdq %%xmm5, 80(%1)\n"
                        "movntdq %%xmm6, 96(%1)\n"
                        "movntdq %%xmm7, 112(%1)\n"
                        :: "r" (from), "r" (to) : "memory");
                from = ((const unsigned char *)from)+128;
                to   = ((unsigned char *)to)+128;
            }
        }
        else
        {
            /*
                  Only if SRC is aligned on 16-byte boundary.
                  It allows to use movaps instead of movups, which required
                  data to be aligned or a general-protection exception (#GP)
                  is generated.
             */
            for (; i>0; i--)
            {
                __asm__ __volatile__ (
                        "prefetchnta 640(%0)\n"

                        "movapd (%0), %%xmm0\n"
                        "movapd 16(%0), %%xmm1\n"
                        "movapd 32(%0), %%xmm2\n"
                        "movapd 48(%0), %%xmm3\n"

                        "movntdq %%xmm0, (%1)\n"
                        "movntdq %%xmm1, 16(%1)\n"
                        "movntdq %%xmm2, 32(%1)\n"
                        "movntdq %%xmm3, 48(%1)\n"

                        "movapd 64(%0), %%xmm4\n"
                        "movapd 80(%0), %%xmm5\n"
                        "movapd 96(%0), %%xmm6\n"
                        "movapd 112(%0), %%xmm7\n"

                        "movntdq %%xmm4, 64(%1)\n"
                        "movntdq %%xmm5, 80(%1)\n"
                        "movntdq %%xmm6, 96(%1)\n"
                        "movntdq %%xmm7, 112(%1)\n"
                        :: "r" (from), "r" (to) : "memory");
                from = ((const unsigned char *)from)+128;
                to   = ((unsigned char *)to)+128;
            }
        }
        /* since movntq is weakly-ordered, a "sfence"
         * is needed to become ordered again. */
        __asm__ __volatile__ ("mfence":::"memory");
        /* enables to use FPU */
        __asm__ __volatile__ ("emms":::"memory");
    }
    /*
     * Now do the tail of the block
     */
    if (len) kernel_memcpy(to, from, len);
    return retval;
}
