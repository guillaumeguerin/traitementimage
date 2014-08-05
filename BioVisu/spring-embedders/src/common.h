// Pestova Valentina UFR

#ifndef COMMON_H
#define COMMON_H

#include <iostream>

// macro to check the CUDA errors
#define CUDA_SAFE_CALL(CALL) {        \
  int cuda_safe_call_retval = (CALL); \
  if(cuda_safe_call_retval!=0) {      \
  std::cerr << "error in " << __FILE__ << ":" << __LINE__<< " -> " << cuda_safe_call_retval << " when calling " << #CALL << "\n"; } }

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

static double Timer_getTime(void)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (double)tv.tv_sec + 1.e-6 * (double)tv.tv_usec;
}

typedef double Timer;

static void startTimer(Timer& timer) { timer = Timer_getTime(); }
static void stopTimer(Timer& timer) { timer = Timer_getTime() - timer; }

#endif
