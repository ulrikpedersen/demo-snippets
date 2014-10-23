/*
 * fcpy_perf.cpp
 *
 *  Created on: 22 Oct 2014
 *      Author: up45
 */
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>

#include <unistd.h>

#include "fastcpy.h"
#include "timestamp.h"

using namespace std;


int main( int argc, char* argv[] )
{
    long pagesize = sysconf(_SC_PAGESIZE);
    cout << "Pagesize: " << pagesize << endl;

    double dt;
    unsigned long int iterations = 1000000;
    int i = 0;
    size_t bufsize = 16*1024;
    double sum_copy_megabytes = (bufsize/ (1024. * 1024))*iterations;

    cout << fixed << setprecision(1)
         << "Copy chunk size:    " << bufsize/1024. << " KB" << endl
         << "Full dataset size:  " << sum_copy_megabytes/1024. << " GB" << endl;

    size_t cachelimit = GetMemcpyCacheLimit();
    cout << fixed << setprecision(1)
         << "asmlib memcpy cache limit: " << cachelimit/(1024.) << " KB" << endl;


    //void * src = calloc(bufsize, sizeof(char));
    void *src;
    posix_memalign(&src, pagesize, bufsize);
    for (int j = 0; j<bufsize/sizeof(int); j++) {
        *((int*) src + j) = rand();
    }
    //void * dest = calloc(bufsize, sizeof(char));
    void * dest;
    posix_memalign(&dest, pagesize, bufsize);

    TimeStamp ts;
    ts.reset();
    for (i = 0; i < iterations; i++) {
        basic_memcpy(dest, src, bufsize);
    }
    dt = ts.seconds_until_now();

    cout << setprecision(3) << fixed
         << "basic: "
         << "Time: " << dt << " sec "
         << "Rate: " << sum_copy_megabytes/dt << " MB/s" << endl;

    ts.reset();
    for (i = 0; i < iterations; i++) {
        dvik_memcpy(dest, src, bufsize);
    }
    dt = ts.seconds_until_now();

    cout << setprecision(3) << fixed
         << "dvik:  "
         << "Time: " << dt << " sec "
         << "Rate: " << sum_copy_megabytes/dt << " MB/s" << endl;

    ts.reset();
    for (i = 0; i < iterations; i++) {
        A_memcpy(dest, src, bufsize);
    }
    dt = ts.seconds_until_now();

    cout << setprecision(3) << fixed
         << "asm:   "
         << "Time: " << dt << " sec "
         << "Rate: " << sum_copy_megabytes/dt << " MB/s" << endl;

    ts.reset();
    for (i = 0; i < iterations; i++) {
        kernel_memcpy(dest, src, bufsize);
    }
    dt = ts.seconds_until_now();

    cout << setprecision(3) << fixed
         << "knl:   "
         << "Time: " << dt << " sec "
         << "Rate: " << sum_copy_megabytes/dt << " MB/s" << endl;

    ts.reset();
    for (i = 0; i < iterations; i++) {
        sse2_memcpy(dest, src, bufsize);
    }
    dt = ts.seconds_until_now();

    cout << setprecision(3) << fixed
         << "sse2:  "
         << "Time: " << dt << " sec "
         << "Rate: " << sum_copy_megabytes/dt << " MB/s" << endl;

    return 0;
}

