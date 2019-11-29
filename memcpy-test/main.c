#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "test.h"

// This need to be updated in metrics script as well
#define BATCH 1000000
#define PAYLOAD_SIZE 10
#define CHUNK_SIZE 1000

BEGIN_TEST_SUIT("memcpy")

long start_time;
char *payload = malloc(sizeof(char) * CHUNK_SIZE * PAYLOAD_SIZE);

for (int r = 0; r < BATCH; r++)
{
  BEGIN_TEST_CASE("without_memcpy", long, "%ld")
  {
    void **blocks = malloc(sizeof(void *) * PAYLOAD_SIZE);
    char *cursor = payload;
    start_time = get_cpu_time();
    for (int i = 0; i < PAYLOAD_SIZE; i++)
    {
      blocks[i] = cursor;
      cursor = cursor + CHUNK_SIZE;
    }
    metric = get_cpu_time() - start_time;
    if (metric < 0)
      metric = 0;
    free(blocks);
  }
  END_TEST_CASE

  BEGIN_TEST_CASE("with_memcpy", long, "%ld")
  {
    char *dest = malloc(sizeof(char) * PAYLOAD_SIZE * CHUNK_SIZE);
    char *dest_cursor = dest;
    char *cursor = payload;
    start_time = get_cpu_time();
    for (int i = 0; i < PAYLOAD_SIZE; i++)
    {
      memcpy(dest, cursor, CHUNK_SIZE);
      cursor = cursor + CHUNK_SIZE;
      dest_cursor = dest_cursor + CHUNK_SIZE;
    }
    metric = get_cpu_time() - start_time;
    if (metric < 0)
      metric = 0;
    free(dest);
  }
  END_TEST_CASE
}

free(payload);

END_TEST_SUIT
