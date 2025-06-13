#ifndef CONSOLE_H
#define CONSOLE_H

void
panic_unexpected(void);
void
clip_confirm_exit(const char *ext, const char *dir);
void
clip_info(void);
void
clip_e_error(void);
void
clip_d_error(void);
void
memory_error(void);
void
make_directory_error(const char *path);
void
concat_error(void);
void
rename_error(const char *src, const char *dst);
void
file_exists_error(const char *path);
void
no_matching_files_error(const char *ext, const char *dir);
void
open_dir_error(const char *path);
void
move_file_success(const char *src, const char *dst);

#endif //CONSOLE_H
