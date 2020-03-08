#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>

#include <jansson.h>

#include "config.h"

const char *argp_program_version = "example-c-project " VERSION;
static char doc[] = "Example project in C language and modern packaging";
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {0},
};

typedef struct
{
  bool verbose;
} Arguments;

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  Arguments *arguments = state->input;
  switch (key)
  {
    case 'v':
      arguments->verbose = true;
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
  if (arguments.verbose)
  {
    printf("Verbose output is turned on\n");
  }
  return 0;
}
