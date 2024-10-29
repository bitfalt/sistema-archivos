#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_system.h"

// Global variables (could be made into a context struct if needed)
static File fileTable[MAX_FILES];
static char dataBlocks[MAX_BLOCKS][BLOCK_SIZE];
static int blockStatus[MAX_BLOCKS];  // 0 = free, 1 = occupied
static int fileCount = 0;

void create_file(char *name, int size) {
    if (fileCount >= MAX_FILES) {
        printf("Error: Maximum file limit reached.\n");
        return;
    }

    int requiredBlocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int freeBlocks = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blockStatus[i] == 0) {
            freeBlocks++;
            if (freeBlocks == requiredBlocks) break;
        }
    }

    if (freeBlocks < requiredBlocks) {
        printf("Error: Not enough space.\n");
        return;
    }

    File newFile;
    strcpy(newFile.name, name);
    newFile.size = size;
    newFile.blocksCount = requiredBlocks;

    int found = 0;
    int blockIndex = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blockStatus[i] == 0) {
            if (found == 0) {
                newFile.startBlock = i;
            }
            newFile.blocks[blockIndex++] = i;
            found++;
            if (found == requiredBlocks) {
                for (int j = newFile.startBlock; j < newFile.startBlock + requiredBlocks; j++) {
                    blockStatus[j] = 1;
                }
                break;
            }
        } else {
            found = 0;
        }
    }

    fileTable[fileCount++] = newFile;
    printf("File %s created successfully.\n", name);
}

void write_file(char *name, int offset, char *data) {
    int found = 0;
    for (int i = 0; i < fileCount; i++) {
        if (strcmp(fileTable[i].name, name) == 0) {
            if (offset + strlen(data) > fileTable[i].size) {
                printf("Error: Write exceeds file size.\n");
                return;
            }

            int blockIndex = offset / BLOCK_SIZE;
            int blockOffset = offset % BLOCK_SIZE;
            int dataLen = strlen(data);

            for (int j = 0; j < dataLen; j++) {
                if (blockOffset == BLOCK_SIZE) {
                    blockIndex++;
                    blockOffset = 0;
                }
                dataBlocks[fileTable[i].blocks[blockIndex]][blockOffset++] = data[j];
            }

            printf("Data written to file %s successfully.\n", name);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: File %s not found.\n", name);
    }
}

void read_file_content(char *name, int offset, int size) {
    int found = 0;
    for (int i = 0; i < fileCount; i++) {
        if (strcmp(fileTable[i].name, name) == 0) {
            if (offset + size > fileTable[i].size) {
                printf("Error: Read exceeds file size.\n");
                return;
            }

            int blockIndex = offset / BLOCK_SIZE;
            int blockOffset = offset % BLOCK_SIZE;
            char buffer[size + 1];
            buffer[size] = '\0';

            for (int j = 0; j < size; j++) {
                if (blockOffset == BLOCK_SIZE) {
                    blockIndex++;
                    blockOffset = 0;
                }
                buffer[j] = dataBlocks[fileTable[i].blocks[blockIndex]][blockOffset++];
            }

            printf("Data read from file %s: %s\n", name, buffer);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: File %s not found.\n", name);
    }
}

void list_files() {
    if (fileCount == 0) {
        printf("No files in the system.\n");
        return;
    }
    for (int i = 0; i < fileCount; i++) {
        printf("%s - %d bytes (Blocks: ", fileTable[i].name, fileTable[i].size);
        for (int j = 0; j < fileTable[i].blocksCount; j++) {
            printf("%d%s", fileTable[i].blocks[j], (j == fileTable[i].blocksCount - 1) ? ")\n" : ", ");
        }
    }
}

void delete_file(char *name) {
    int found = 0;
    for (int i = 0; i < fileCount; i++) {
        if (strcmp(fileTable[i].name, name) == 0) {
            for (int j = 0; j < fileTable[i].blocksCount; j++) {
                blockStatus[fileTable[i].blocks[j]] = 0;
            }
            for (int k = i; k < fileCount - 1; k++) {
                fileTable[k] = fileTable[k + 1];
            }
            fileCount--;
            printf("File %s deleted successfully.\n", name);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: File %s not found.\n", name);
    }
}
