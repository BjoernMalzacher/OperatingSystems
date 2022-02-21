/* stat implementation changed in glibc 2.33, so we need to overwrite two functions. */

/* glibc < 2.33 */
int __xstat (int __ver, const char *__filename,
                    struct stat *__stat_buf) {
    (void) __ver;
    (void) __filename;
    __stat_buf->st_blocks = 3;
    __stat_buf->st_size = 123;
    return 0;
}

/* glibc >= 2.33 */
int stat(const char *file, struct stat *buf) {
    (void) file;
    buf->st_blocks = 3;
    buf->st_size = 123;
    return 0;
}

