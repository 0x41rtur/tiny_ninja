#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

char *
copy_str(const char *s);
char *
copy_str_n(const char *s, size_t l);
char *
extension(const char *filename);
int
split(const char *s, char **before, char **after, char d);
int
is_file(const char *filename);
char *
concat(const char *first, ...);

#endif //COMMON_H
