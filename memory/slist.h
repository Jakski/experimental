#include <sys/queue.h>

typedef struct slist_item_s {
  void *value;
  int id;
  SLIST_ENTRY(slist_item_s) entries;
} Slist_item;

SLIST_HEAD(slist_s, slist_item_s);
typedef struct slist_s Slist;
