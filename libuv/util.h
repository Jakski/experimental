#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define LOG_INFO(...) \
    do { \
        fprintf(stdout, "INFO:%s:%d: ", __FILE__, __LINE__); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout, "\n"); \
    } while(0)

#define LOG_ERROR(...) \
    do { \
        fprintf(stderr, "ERROR:%s:%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while(0)

#endif
