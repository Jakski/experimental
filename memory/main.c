#include <stdio.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <time.h>

/*
 * Test set multiplier.
 */
#define MULTIPLIER 100000

typedef struct slist_item_s {
  void *value;
  int id;
  SLIST_ENTRY(slist_item_s) entries;
} Slist_item;

SLIST_HEAD(slist_s, slist_item_s);
typedef struct slist_s Slist;

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

int main(int argc, char **argv)
{
  long start_time = 0;
  long sum_time = 0;
  Slist slist;
  Slist_item *item;
  SLIST_INIT(&slist);

  printf("malloc | Allocate singly-linked list sequentially: ");
  {
    sum_time = 0;
    for (int i = 0; i < MULTIPLIER; i++)
    {
      start_time = get_cpu_time();
      item = malloc(sizeof(Slist_item));
      SLIST_INSERT_HEAD(&slist, item, entries);
      sum_time += get_cpu_time() - start_time;
    }
  }
  printf("%ld\n", sum_time);

  printf("malloc | Free singly-linked list allocated sequentially: ");
  {
    sum_time = 0;
    while (!SLIST_EMPTY(&slist))
    {
      start_time = get_cpu_time();
      item = SLIST_FIRST(&slist);
      free(item);
      SLIST_REMOVE_HEAD(&slist, entries);
      sum_time += get_cpu_time() - start_time;
    }
  }
  printf("%ld\n", sum_time);

  printf("malloc | Allocate singly-linked list non-sequentially: ");
  {
    Slist slist2;
    Slist_item *to_delete = NULL;
    SLIST_INIT(&slist2);
    int base_shard_size = sizeof(char);
    int max_shard_multiplier = 20;
    sum_time = 0;
    for (int i = 0; i < MULTIPLIER; i++)
    {
      item = malloc(sizeof(Slist_item));
      item->value = malloc(base_shard_size * (i % max_shard_multiplier) + 1);
      item->id = i;
      SLIST_INSERT_HEAD(&slist2, item, entries);
    }
    // Iteratively remove every third element. We can't remove element we're
    // inspecting, so free is invoked only on past elements.
    SLIST_FOREACH(item, &slist2, entries)
    {
      if (to_delete != NULL)
      {
        SLIST_REMOVE(&slist2, to_delete, slist_item_s, entries);
        free(to_delete->value);
        free(to_delete);
        to_delete = NULL;
      }
      if (item->id % 3 == 0)
      {
        to_delete = item;
      }
    }
    if (to_delete != NULL)
    {
      SLIST_REMOVE(&slist2, to_delete, slist_item_s, entries);
      free(to_delete->value);
      free(to_delete);
    }

    for (int i = 0; i < MULTIPLIER; i++)
    {
      start_time = get_cpu_time();
      item = malloc(sizeof(Slist_item));
      SLIST_INSERT_HEAD(&slist, item, entries);
      sum_time += get_cpu_time() - start_time;
    }

    // Get rid of temporary sequentially allocated list with holes
    while (!SLIST_EMPTY(&slist2))
    {
      item = SLIST_FIRST(&slist2);
      free(item->value);
      free(item);
      SLIST_REMOVE_HEAD(&slist2, entries);
    }
  }
  printf("%ld\n", sum_time);

  printf("malloc | Free singly-linked list allocated non-sequentially: ");
  {
    sum_time = 0;
    while (!SLIST_EMPTY(&slist))
    {
      start_time = get_cpu_time();
      item = SLIST_FIRST(&slist);
      free(item);
      SLIST_REMOVE_HEAD(&slist, entries);
      sum_time += get_cpu_time() - start_time;
    }
  }
  printf("%ld\n", sum_time);

  return 0;
}
