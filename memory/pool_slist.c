/**
 * Singly-linked list allocation with memory pooling.
 */

#include <sys/queue.h>
#include <stdlib.h>

#include "test.h"
#include "slist.h"

static Slist_item *pool_alloc_item(Slist *pool)
{
  Slist_item *r = SLIST_FIRST(pool);
  if (r == NULL)
  {
    r = xmalloc(sizeof(Slist_item));
  } else {
    SLIST_REMOVE_HEAD(pool, entries);
  }
  return r;
}

static void pool_free_item(Slist *pool, Slist_item *item)
{
  SLIST_INSERT_HEAD(pool, item, entries);
}

static void pool_free(Slist *pool)
{
  Slist_item *item;
  while (!SLIST_EMPTY(pool))
  {
    item = SLIST_FIRST(pool);
    SLIST_REMOVE_HEAD(pool, entries);
    free(item);
  }
}

BEGIN_TEST_SUIT("pool_slist")

long start_time;
Slist slist, slist_item_pool;
Slist_item *item;
SLIST_INIT(&slist);
SLIST_INIT(&slist_item_pool);

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
