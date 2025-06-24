#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

char *
strcopy(const char *s);
char *
strncopy(const char *s, size_t l);
char *
find_ext_suffix(const char *filename);
int
split(const char *s, char **before, char **after, char d);
int
is_regular_file(const char *filename);
char *
concat(const char *first, ...);
char* strtok_iso(char *str, const char* delim, char **saverptr);
char **strtokarr(const char *str, int delim);

#endif //COMMON_H
