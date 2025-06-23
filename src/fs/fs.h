#ifndef FS_H
#define FS_H

struct target
{
        char           *name;
        struct command *cmd;
};

struct target **
find_target(const struct command *cmd);
int
mk_dir(const char *dir);
int
rm_dir(const char *dir);
int
is_target(const char *filename, const char *target);
int
make_dir_recursive(const char *dir);

#endif //FS_H
