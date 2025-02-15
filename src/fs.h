/* fs.h */
#ifndef FS_H
#define FS_H

#include <stdint.h>

void parse_cpio_archive(void *start, uint32_t size);

#endif
