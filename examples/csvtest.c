/*
csvtest - reads CSV data from stdin and output properly formed equivalent
          useful for testing the library
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <csv.h>

static int put_comma;

void cb1 (void *s, size_t i, void *p) {
  if (put_comma)
    putc(',', stdout);
  csv_fwrite(stdout, s, i);
  put_comma = 1;
}

void cb2 (int c, void *p) {
  put_comma = 0;
  putc('\n', stdout);
}

int main (void) {
  struct csv_parser p;
  int i;
  char c;

  csv_init(&p, 0);

  while ((i=getc(stdin)) != EOF) {
    c = i;
    if (csv_parse(&p, &c, 1, cb1, cb2, NULL) != 1) {
      fprintf(stderr, "Error: %s\n", csv_strerror(csv_error(&p)));
      exit(EXIT_FAILURE);
    }
  }

  csv_fini(&p, cb1, cb2, NULL);
  csv_free(&p);

  return EXIT_SUCCESS;
}

