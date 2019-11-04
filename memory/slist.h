#include <sys/queue.h>

typedef struct slist_item_s {
  SLIST_ENTRY(slist_item_s) entries;
  void *value;
} Slist_item;

SLIST_HEAD(slist_s, slist_item_s);
typedef struct slist_s Slist;
