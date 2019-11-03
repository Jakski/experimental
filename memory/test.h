#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

/**
 * Mini-framework for testing performance.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * CPU time may wrap, so it's good to measure with more samples.
 */
static long get_cpu_time(void)
{
  struct timespec time;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time) < 0)
  {
    fprintf(stderr, "Failed to get CPU time!\n");
    exit(1);
  }
  return time.tv_nsec;
}

void *xmalloc(size_t size)
{
  void *r = malloc(size);
  if (r == NULL)
  {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(1);
  }
  return r;
}

/**
 * How many times to repeat measured operations in tests.
 */
#ifndef TEST_MULTIPLIER
#define TEST_MULTIPLIER 10000
#endif
#define BEGIN_TEST_SUIT(title) \
  int main(int argc, char **argv) {\
    static const char \
      *_test_case_name = NULL,\
      *_test_case_format = NULL,\
      *_test_suit_name = title;
#define END_TEST_SUIT }
#define BEGIN_TEST_CASE(title, type, format) \
  {\
    type metric = 0;\
    _test_case_name = title;\
    _test_case_format = format;
#define END_TEST_CASE \
    printf("%s.%s ", _test_suit_name, _test_case_name);\
    printf(_test_case_format, metric);\
    printf("\n");\
  }

#endif
