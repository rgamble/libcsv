/*
csvvalid - determine if files are properly formed CSV files and display
           position of first offending byte if not
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <csv.h>

int
main (int argc, char *argv[])
{
  FILE *fp;
  int i;
  struct csv_parser p;
  char buf[1024];
  size_t bytes_read;
  size_t pos;
  size_t retval;

  if (argc < 2) {
    fprintf(stderr, "Usage: csvvalid files\n");
    exit(EXIT_FAILURE);
  }

  if (csv_init(&p, CSV_STRICT) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    exit(EXIT_FAILURE);
  }

  for (i = 1; i < argc; i++) {
    pos = 0;
    fp = fopen(argv[i], "rb");
    if (!fp) {
      fprintf(stderr, "Failed to open %s: %s, skipping\n", argv[i], strerror(errno));
      continue;
    }
    while ((bytes_read=fread(buf, 1, 1024, fp)) > 0) {
      if ((retval = csv_parse(&p, buf, bytes_read, NULL, NULL, NULL)) != bytes_read) {
        if (csv_error(&p) == CSV_EPARSE) {
          printf("%s: malformed at byte %lu\n", argv[i], (unsigned long)pos + retval + 1);
          goto end;
        } else {
          printf("Error while processing %s: %s\n", argv[i], csv_strerror(csv_error(&p)));
          goto end;
        }
      }
      pos += bytes_read;
    }
    printf("%s well-formed\n", argv[i]);

    end:
    fclose(fp);
    csv_fini(&p, NULL, NULL, NULL);
    pos = 0;
  }

  csv_free(&p);
  return EXIT_SUCCESS;
}
