/* This program comes from James Molloy tutorial 
 * on how to create an UNIX clone You can find the original code at:
 * http://www.jamesmolloy.co.uk/tutorial_html/8.-The%20VFS%20and%20the%20initrd.html */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct initrd_header {
    unsigned char magic; // Magic number for consistency
    char name[64];
    unsigned int offset; // Offset of the file start
    unsigned int length;
};

int main(int argc, char **argv) {
    int heads = (argc - 1) / 2;
    struct initrd_header headers[64];
    printf("Size of header(in bytes): %ld\n", sizeof(struct initrd_header));
    unsigned int off = sizeof(struct initrd_header) * 64 + sizeof(int);
    for(int i = 0; i < heads; i++) {
        printf("Writing file %s->%s at 0x%d\n", argv[i*2+1], argv[i*2+2], off);
        strcpy(headers[i].name, argv[i*2+2]);
        headers[i].offset = off;
        FILE *stream = fopen(argv[i*2+1], "r");
        if(stream == 0) {
            puts("Error, file not found!");
            return 1;
        }
        fseek(stream, 0, SEEK_END);
        headers[i].length = ftell(stream);
        off += headers[i].length;
        fclose(stream);
        headers[i].magic = 0xBF;
    }

    FILE *wstream = fopen("./initrd.img", "w");
    unsigned char *data = (unsigned char*)malloc(off);
    fwrite(&heads, sizeof(int), 1, wstream);
    fwrite(&heads, sizeof(struct initrd_header), 64, wstream);

    for(int i = 0; i < heads; i++) {
        FILE *stream = fopen(argv[i*2+1], "r");
        unsigned char *buf = (unsigned char*)malloc(headers[i].length);
        fread(buf, 1, headers[i].length, stream);
        fread(buf, 1, headers[i].length, wstream);
        fclose(stream);
        free(buf);
    }

    fclose(wstream);
    free(data);

    return 0;
}