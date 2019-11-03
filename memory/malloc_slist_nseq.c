#include <sys/queue.h>
#include <stdlib.h>

#include "test.h"
#include "slist.h"

/**
 * Singly-linked list non-sequential allocation with malloc.
 */

BEGIN_TEST_SUIT("malloc_slist_nseq")

long start_time;
Slist slist;
Slist_item *item;
SLIST_INIT(&slist);

BEGIN_TEST_CASE("allocation", long, "%ld")
{
  void *value = NULL;
  for (int i = 0; i < TEST_MULTIPLIER; i++)
  {
    value = xmalloc((i % 50) + 1);
    start_time = get_cpu_time();
    item = xmalloc(sizeof(Slist_item));
    SLIST_INSERT_HEAD(&slist, item, entries);
    metric += get_cpu_time() - start_time;
    item->value = value;
  }
}
END_TEST_CASE

BEGIN_TEST_CASE("deallocation", long, "%ld")
{
  void *value = NULL;
  while (!SLIST_EMPTY(&slist))
  {
    start_time = get_cpu_time();
    item = SLIST_FIRST(&slist);
    value = item->value;
    free(item);
    SLIST_REMOVE_HEAD(&slist, entries);
    metric += get_cpu_time() - start_time;
    free(value);
  }
}
END_TEST_CASE

END_TEST_SUIT
