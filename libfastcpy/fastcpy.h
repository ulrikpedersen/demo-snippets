/*
 * fastcpy.h
 *
 *  Created on: 22 Oct 2014
 *      Author: up45
 */

#ifndef LIBFASTCPY_FASTCPY_H_
#define LIBFASTCPY_FASTCPY_H_

#include <cstring>

#include "dvik_memcpy.h"

inline void *basic_memcpy(void *dest, const void *src, size_t n) {
    return memcpy(dest, src, n);
}

void *duff_memcpy(void *dest, const void *src, size_t n);


#endif /* LIBFASTCPY_FASTCPY_H_ */
