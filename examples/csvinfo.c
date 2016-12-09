/*
csvinfo - reads CSV data from input file(s) and reports the number
          of fields and rows encountered in each file
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "libcsv/csv.h"

struct counts {
  long unsigned fields;
  long unsigned rows;
};

void cb1 (void *s, size_t len, void *data) { ((struct counts *)data)->fields++; }
void cb2 (int c, void *data) { ((struct counts *)data)->rows++; }

static int is_space(unsigned char c) {
  if (c == CSV_SPACE || c == CSV_TAB) return 1;
  return 0;
}

static int is_term(unsigned char c) {
  if (c == CSV_CR || c == CSV_LF) return 1;
  return 0;
}


int
main (int argc, char *argv[])
{
  FILE *fp;
  struct csv_parser p;
  char buf[1024];
  size_t bytes_read;
  unsigned char options = 0;
  struct counts c = {0, 0};

  if (argc < 2) {
    fprintf(stderr, "Usage: csvinfo [-s] files\n");
    exit(EXIT_FAILURE);
  }

  if (csv_init(&p, options) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    exit(EXIT_FAILURE);
  }

  csv_set_space_func(&p, is_space);
  csv_set_term_func(&p, is_term);

  while (*(++argv)) {
    if (strcmp(*argv, "-s") == 0) {
      options = CSV_STRICT;
      csv_set_opts(&p, options);
      continue;
    }

    fp = fopen(*argv, "rb");
    if (!fp) {
      fprintf(stderr, "Failed to open %s: %s\n", *argv, strerror(errno));
      continue;
    }

    while ((bytes_read=fread(buf, 1, 1024, fp)) > 0) {
      if (csv_parse(&p, buf, bytes_read, cb1, cb2, &c) != bytes_read) {
        fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(&p)));
      }
    }

    csv_fini(&p, cb1, cb2, &c);

    if (ferror(fp)) {
      fprintf(stderr, "Error while reading file %s\n", *argv);
      fclose(fp);
      continue;
    }

    fclose(fp);
    printf("%s: %lu fields, %lu rows\n", *argv, c.fields, c.rows);
  }

  csv_free(&p);
  exit(EXIT_SUCCESS);
}
 
