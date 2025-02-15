/* fs.c */
#include <stdint.h>
#include "fs.h"
#include "print.h"  // For printf and putchar

// Minimal implementations of memcmp, strcmp, and strlen for a freestanding environment.
int memcmp(const void *s1, const void *s2, uint32_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

uint32_t strlen(const char *s) {
    uint32_t len = 0;
    while (s[len])
        len++;
    return len;
}

// Structure for CPIO newc header (fixed-length fields in ASCII hexadecimal).
typedef struct {
    char c_magic[6];
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
} cpio_newc_header_t;

// Helper: Convert an ASCII hex string of fixed length to an integer.
static uint32_t hex2int(const char *str, int len) {
    uint32_t val = 0;
    for (int i = 0; i < len; i++) {
        char c = str[i];
        val <<= 4;
        if (c >= '0' && c <= '9')
            val += c - '0';
        else if (c >= 'a' && c <= 'f')
            val += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            val += c - 'A' + 10;
    }
    return val;
}

// Parse the CPIO archive in newc format.
// 'start' points to the beginning of the archive and 'size' is its total size.
void parse_cpio_archive(void *start, uint32_t size) {
    uint8_t *ptr = (uint8_t *)start;
    uint8_t *end = ptr + size;
    
    while (ptr < end) {
        cpio_newc_header_t *hdr = (cpio_newc_header_t *)ptr;
        
        // Check for valid magic number ("070701")
        if (memcmp(hdr->c_magic, "070701", 6) != 0) {
            printf("Invalid cpio magic. Stopping parse.\n");
            return;
        }
        
        // Convert fields from ASCII hex to integers.
        uint32_t namesize = hex2int(hdr->c_namesize, 8);
        uint32_t filesize = hex2int(hdr->c_filesize, 8);
        
        // Move pointer past header.
        ptr += sizeof(cpio_newc_header_t);
        
        // Filename is next.
        char *filename = (char *)ptr;
        
        // If this is the special "TRAILER!!!" entry, we're done.
        if (strcmp(filename, "TRAILER!!!") == 0) {
            printf("Reached end of CPIO archive.\n");
            return;
        }
        
        printf("File: %s, size: %d bytes\n", filename, filesize);
        
        // Align namesize to a multiple of 4 bytes.
        uint32_t aligned_namesize = (namesize + 3) & ~3;
        ptr += aligned_namesize;
        
        // The file data is next.
        uint8_t *file_data = ptr;
        
        // Print file contents (assumes text file).
        printf("Contents: ");
        for (uint32_t i = 0; i < filesize; i++) {
            putchar(file_data[i]);
        }
        putchar('\n');
        
        // Align file size to a multiple of 4 bytes.
        uint32_t aligned_filesize = (filesize + 3) & ~3;
        ptr += aligned_filesize;
    }
}
