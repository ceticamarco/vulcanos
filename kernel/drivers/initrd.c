#include "initrd.h"

// Declare various things
initrd_header_t *initrd_header; // Header
initrd_file_header_t *file_header; // List of headers
fs_node_t *initrd_root; // Directory root node
fs_node_t *initrd_dev; // Directory dev node(/dev)
fs_node_t *root_nodes; // List of file nodes
uint32_t nroot_nodes;

struct dirent dirent;

fs_node_t *init_ramdisk(uint32_t multiboot_location) {
    // Initialize main and file headers
    initrd_header = (initrd_header_t*)multiboot_location;
    file_header = (initrd_file_header_t*)(multiboot_location+sizeof(initrd_header_t));
    // Initialize root directory
    initrd_root = (fs_node_t*)

}

static uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    initrd_file_header_t header = file_header[node->inode];
    if(offset > header.length)
        return 0;
    if(offset+size > header.length)
        size = header.length-offset;
    memcpy(buffer, (uint8_t*) (header.offset+offset), size);
    return size;
}

static struct dirent *initrd_readdir(fs_node_t *node, uint32_t index) {
    if(node == initrd_root && index == 0) {
        strcpy(dirent.name, "dev");
        dirent.name[3] = 0; // Add null terminator to the string
        dirent.ino = 0;
        return &dirent;
    }

    if(index-1 >= &root_nodes)
        return 0;
    strcpy(dirent.name, root_nodes[index-1].name);
    dirent.name[strlen(root_nodes[index-1].name)] = 0; // Add null terminator
    dirent.ino = root_nodes[index-1].inode;
    return &dirent;
}

static fs_node_t *initrd_finddir(fs_node_t *node, uint8_t *name) {
    if(node == initrd_root && !strcmp(name, "dev"))
        return initrd_dev;
    
    for(uint32_t i = 0; i < nroot_nodes; i++)
        if(!strcmp(name, root_nodes[i].name))
            return &root_nodes[i];
    return 0;
}