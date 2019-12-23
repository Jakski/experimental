#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <search.h>

typedef struct {
  int x;
  int y;
} coord_t;

typedef struct {
  coord_t coord;
  int lstr;
  bool candle;
} node_t;

static int compare(const void *a, const void *b)
{
  coord_t *acoord = (coord_t *) a;
  coord_t *bcoord = (coord_t *) b;
  return memcmp(acoord, bcoord, sizeof(coord_t));
}

int main(int argc, char **argv)
{
  void *root = NULL;
  coord_t coord;
  node_t *node = NULL;
  node_t **found = NULL;

  node = malloc(sizeof(node_t));
  node->lstr = 1;
  node->candle = false;
  node->coord.x = 0;
  node->coord.y = 1;
  found = tsearch(&node->coord, &root, &compare);
  *found = node;

  node = malloc(sizeof(node_t));
  node->lstr = 2;
  node->candle = true;
  node->coord.x = 1;
  node->coord.y = 2;
  found = tsearch(&node->coord, &root, &compare);
  *found = node;

  coord.x = 1;
  coord.y = 2;
  found = tfind(&coord, &root, &compare);
  if (found == NULL)
  {
    printf("Couldn't find a candle\n");
    exit(EXIT_FAILURE);
  }
  if ((*found)->candle == true)
    printf("Found a candle\n");

  tdestroy(root, free);
  return 0;
}
