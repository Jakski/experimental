#include <stdio.h>

#include "json.h"

int main(int argc, char **argv)
{
  const char text[] = "{\"test\": \"lol\"}";
  if(!print_json_key(text, "test"))
    return 1;

  if(print_json_key(text, "nonexistent"))
    return 1;

  return 0;
}
