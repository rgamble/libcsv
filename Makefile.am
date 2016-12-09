lib_LTLIBRARIES = libcsv.la
     libcsv_la_SOURCES = libcsv.c
     libcsv_la_LDFLAGS = -version-info 3:2:0
     libcsv_la_CFLAGS = -Wall -Wextra 
libcsv_includedir = $(includedir)
nobase_libcsv_include_HEADERS = csv.h
dist_man_MANS = csv.3
EXTRA_DIST = FAQ \
README \
test_csv.c \
tests \
examples \
csv.pdf \
COPYING.LESSER
TESTS = check_csv
check_PROGRAMS = check_csv
check_csv_SOURCES = test_csv.c
check_csv_LDADD = -lcsv
ACLOCAL_AMFLAGS = -I m4
