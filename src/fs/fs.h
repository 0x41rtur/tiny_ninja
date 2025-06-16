#ifndef FS_H
#define FS_H

struct tnt
{
        char *          name;
        struct command *cmd;
};

struct tnt **
find_tnt(const struct command *cmd);
int
mk_dir(const char *dir);
int
rm_dir(const char *dir);
int
is_tnt(const char *filename, const char *target);

#endif //FS_H
