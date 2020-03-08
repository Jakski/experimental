#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>

#include "config.h"
#include "json.h"

const char *argp_program_version = "example-c-project " VERSION;
static char doc[] = "Example project in C language and modern packaging";
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"input", 'i', 0, 0, "Input JSON file"},
  {"key", 'k', 0, 0, "JSON key to display"},
  {0},
};

typedef struct
{
  bool verbose;
  char *input_file;
  char *key;
} Arguments;

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  Arguments *arguments = state->input;
  switch(key)
  {
    case 'v':
      arguments->verbose = true;
      break;
    case 'i':
      arguments->input_file = arg;
      break;
    case 'k':
      arguments->key = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

int main(int argc, char **argv)
{
  Arguments arguments = {false};
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if(arguments.verbose)
  {
    printf("Verbose output is turned on\n");
  }

  char text[] = "{\"test\": \"lol\"}";
  if(!print_json_key(text, "tests"))
  {
    fprintf(stderr, "Failed to print key!\n");
    return 1;
  }

  return 0;
}
