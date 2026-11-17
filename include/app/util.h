#ifndef APP_UTIL_H
#define APP_UTIL_H

// reads the whole file into a malloc'd, NUL-terminated buffer (caller frees)
char *read_file(const char *path);

#endif
