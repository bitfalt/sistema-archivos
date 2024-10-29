#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#define MAX_FILES 100
#define MAX_BLOCKS 2048
#define BLOCK_SIZE 512

typedef struct {
    char name[50];
    int size;
    int startBlock;
    int blocksCount;
    int blocks[MAX_BLOCKS];
} File;

// Core file system operations
void create_file(char *name, int size);
void write_file(char *name, int offset, char *data);
void read_file_content(char *name, int offset, int size);
void list_files(void);
void delete_file(char *name);

#endif // FILE_SYSTEM_H