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

