/*****************************************
 *          VulcanOS Kernel              *
 * Developed by Marco 'icebit' Cetica    * 
 *            (c) 2019-2021              *
 *       Released under GPLv3            *
 * https://github.com/ice-bit/vulcanos   *
 *****************************************/
#ifndef INITRD_H
#define INITRD_H
/* Ramdisk is a file system that is loaded along with the kernel
 * and it usually contains configuration files or file system drivers.
 * It is used BEFORE partitions are being mounted*/

#include <stdint.h>
#include "../libc/string.h"
#include "../mem/kheap.h"
#include "fs.h"

typedef struct {
    uint32_t nfiles; // Number of files in the ramdisk
} initrd_header_t;

typedef struct {
    uint8_t magic; // Magic number for error checking
    int8_t name[64]; // Filename
    uint32_t offset;
    uint32_t length; // Length of the file
} initrd_file_header_t;

// Function to initialize initrd, we'll pass the multiboot
// module as parameter
fs_node_t *init_ramdisk(uint32_t multiboot_location);

#endif
