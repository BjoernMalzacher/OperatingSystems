#ifndef FSSIZE_H
#define FSSIZE_H

#include <dirent.h>
#include <sys/types.h>

#ifndef ALLOW_OVERRIDE
#define ALLOW_OVERRIDE __attribute__((weak))
#endif

#define MAX_FILES 4096
#define NUM_THREADS 4

void listDir(const char *dirpath);
/* Necessary for test_list_dir */
ALLOW_OVERRIDE
void processEntry(const char *dirpath, struct dirent *entry);
/* Necessary for test_process_entry */
ALLOW_OVERRIDE
void processFile(const char *path);
/* Necessary for test_process_entry and test_get_filesystem_size_init*/
ALLOW_OVERRIDE
void addDirectory(const char *dirpath);
char *makePath(const char *dirpath, const char *name);
unsigned long long getFilesystemSize(char *path);
/* Necessary for test_process_file_sync */
ALLOW_OVERRIDE
int tableContains(ino_t inode);
/* Necessary for test_process_file_sync */
ALLOW_OVERRIDE
void tableInsert(ino_t inode);
#endif
