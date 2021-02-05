#include "initrd.h"

// Private API to IO operations on dir and files
static uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
static struct dirent *initrd_readdir(fs_node_t *node, uint32_t index);
static fs_node_t *initrd_finddir(fs_node_t *node, uint8_t *name);

// Declare various things
initrd_header_t *initrd_header; // Header
initrd_file_header_t *file_headers; // List of headers
fs_node_t *initrd_root; // Directory root node
fs_node_t *initrd_dev; // Directory dev node(/dev)
fs_node_t *root_nodes; // List of file nodes
uint32_t nroot_nodes; // Number of file nodes

struct dirent dirent;

fs_node_t *init_ramdisk(uint32_t multiboot_location) {
    // Initialize main and file headers
    initrd_header = (initrd_header_t*)multiboot_location;
    file_headers = (initrd_file_header_t*)(multiboot_location+sizeof(initrd_header_t));
    // Initialize root directory
    initrd_root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = 0;
    initrd_root->write = 0;
    initrd_root->open = 0;
    initrd_root->close = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = 0;
    initrd_root->impl = 0;

    // Initialize the /dev/ directory
    initrd_dev = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "dev");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = 0;
    initrd_root->write = 0;
    initrd_root->open = 0;
    initrd_root->close = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = 0;
    initrd_root->impl = 0;

    // Add dev and root directories to the ramdisk
    // Let us first allocate space for them
    root_nodes = (fs_node_t*)kmalloc(sizeof(fs_node_t) *initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;
    // and then, add each file to an inode
    for(uint32_t i = 0; i < initrd_header->nfiles; i++) {
        file_headers[i].offset += multiboot_location;
        // Create an inode for the file
        strcpy(root_nodes[i].name, &file_headers[i].name);
        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].length = file_headers[i].length;
        root_nodes[i].inode = i;
        root_nodes[i].flags = FS_FILE;
        root_nodes[i].read = &initrd_read;
        root_nodes[i].write = 0;
        root_nodes[i].readdir = 0;
        root_nodes[i].finddir = 0;
        root_nodes[i].open = 0;
        root_nodes[i].close = 0;
        root_nodes[i].impl = 0;
    }

    return initrd_root;
}

static uint32_t initrd_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    initrd_file_header_t header = file_headers[node->inode];
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