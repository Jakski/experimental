/**
 * Singly-linked list allocation with chunked memory pooling.
 */

#include <sys/queue.h>
#include <stdlib.h>

#include "test.h"
#include "slist.h"

typedef struct {
  Slist chunks;
  Slist free;
} Pool;

static void pool_init(Pool *pool)
{
  SLIST_INIT(&pool->chunks);
  SLIST_INIT(&pool->free);
}

static Slist_item *pool_alloc_item(Pool *pool)
{
  Slist_item *r = SLIST_FIRST(&pool->free);
  if (r == NULL)
  {
    Slist_item *chunk = xmalloc(sizeof(Slist_item) * CHUNK_SIZE);
    Slist_item *chunk_item = xmalloc(sizeof(Slist_item));
    chunk_item->value = chunk;
    SLIST_INSERT_HEAD(&pool->chunks, chunk_item, entries);
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
      SLIST_INSERT_HEAD(&pool->free, &chunk[i], entries);
    }
    r = SLIST_FIRST(&pool->free);
  }
  SLIST_REMOVE_HEAD(&pool->free, entries);
  return r;
}

static void pool_free_item(Pool *pool, Slist_item *item)
{
  SLIST_INSERT_HEAD(&pool->free, item, entries);
}

static void pool_free(Pool *pool)
{
  Slist_item *item;
  while (!SLIST_EMPTY(&pool->chunks))
  {
    item = SLIST_FIRST(&pool->chunks);
    SLIST_REMOVE_HEAD(&pool->chunks, entries);
    free(item->value);
    free(item);
  }
}

BEGIN_TEST_SUIT(TEST_SUIT_NAME)

long start_time;
Slist slist;
Pool slist_item_pool;
Slist_item *item;
SLIST_INIT(&slist);
pool_init(&slist_item_pool);

BEGIN_TEST_CASE("primary_allocation", long, "%ld")
{
  for (int i = 0; i < TEST_MULTIPLIER; i++)
  {
    start_time = get_cpu_time();
    item = pool_alloc_item(&slist_item_pool);
    SLIST_INSERT_HEAD(&slist, item, entries);
    metric += get_cpu_time() - start_time;
  }
}
END_TEST_CASE

BEGIN_TEST_CASE("items_deallocation", long, "%ld")
{
  while (!SLIST_EMPTY(&slist))
  {
    start_time = get_cpu_time();
    item = SLIST_FIRST(&slist);
    SLIST_REMOVE_HEAD(&slist, entries);
    pool_free_item(&slist_item_pool, item);
    metric += get_cpu_time() - start_time;
  }
}
END_TEST_CASE

BEGIN_TEST_CASE("secondary_allocation", long, "%ld")
{
  for (int i = 0; i < TEST_MULTIPLIER; i++)
  {
    start_time = get_cpu_time();
    item = pool_alloc_item(&slist_item_pool);
    SLIST_INSERT_HEAD(&slist, item, entries);
    metric += get_cpu_time() - start_time;
  }
}
END_TEST_CASE

BEGIN_TEST_CASE("full_deallocation", long, "%ld")
{
  while (!SLIST_EMPTY(&slist))
  {
    start_time = get_cpu_time();
    item = SLIST_FIRST(&slist);
    SLIST_REMOVE_HEAD(&slist, entries);
    pool_free_item(&slist_item_pool, item);
    metric += get_cpu_time() - start_time;
  }
  start_time = get_cpu_time();
  pool_free(&slist_item_pool);
  metric += get_cpu_time() - start_time;
}
END_TEST_CASE

END_TEST_SUIT
