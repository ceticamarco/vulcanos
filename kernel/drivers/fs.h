/**************************************
 *          iceOS Kernel              *
 * Developed by Marco 'icebit' Cetica * 
 *            (c) 2019                *
 *       Released under GPLv3         *
 * https://github.com/ice-bit/iceOS   *
 ***************************************/
#ifndef FS_H
#define FS_H

#include <stdint.h>
/* This Virtual File System(VFS) is a simplified version of 
 * standard UNIX VFS where all files comes organized in a graph
 * of nodes. Keeping in mind the concept of "everything is a file" we can
 * store a file, a directory, a serial device or anything else just by adding
 * a new node to the data structure. 
 * And this is a list of common operations: 
 * - Open: Initialize a new node as a file descriptor
 * - Close: CLose a node 
 * - Read: return the content of a node
 * - Write: set content to node
 * - Readdir: Return directory content
 * - Finddir: Given a specific name find the corresponding child node.*/

// Define some standard node types
#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08

struct fs_node;

/* Define some callbacks to be called when read/write/open/close
 * operations are called */
typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent*(*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node*(*finddir_type_t)(struct fs_node*, char *name);

// This define the structure of a node
typedef struct fs_node {
    uint8_t name[128]; // File name
    uint32_t mask; // Permission mask
    uint32_t uid; // Owning user
    uint32_t gid; // Owning group
    uint32_t flags; // Node type
    uint32_t inode; // used by file systems to identify files
    uint32_t length; // Length of the file, in bytes.
    uint32_t impl;
    // Callback section
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    struct fs_node *ptr; // Used by mountpoints and symlinks
} fs_node_t;

struct dirent {
    uint8_t name[120]; // File name
    uint32_t ino; // POSIX standard requires inode number;
};

// Filesystem root
extern fs_node_t *fs_root;

/* Write/Read/Open/Close operations
 * NOTE: those functions are NOT like the Callback
 * functions; the first one deals with inodes while
 * the second one deals with file descriptors. */
uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(fs_node_t *node);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);

#endif