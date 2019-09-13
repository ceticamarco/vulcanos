#include "fs.h"

fs_node_t *fs_root = 0; // Initialize the root of the filesystem

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    // Check if inode got a read callback from the kernel
    if(node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    // Check if inode got a write callback from the kernel
    if(node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

void open_fs(fs_node_t *node, uint8_t read, uint8_t write) {
    // Check if inode got a open callback from the kernel
    if(node->open != 0)
        return node->open(node);
    else
        return 0;
}

void close_fs(fs_node_t *node) {
    // Check if inode got a close callback from the kernel
    if(node->close =! 0)
        return node->close(node);
    else
        return 0;
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t index) {
    // Read dir content(only if file descriptor is FS_DIRECTORY)
    if((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0)
        return node->readdir(node, index);
    else
        return 0;
}

fs_node_t *finddir_fs(fs_node_t *node, char *name) {
    // Check if an inode is a directory
    if((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0) 
        return node->finddir(node, name);
    else
        return 0;
}