#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

File fileTable[MAX_FILES];
char dataBlocks[MAX_BLOCKS][BLOCK_SIZE];
int blockStatus[MAX_BLOCKS];  // 0 = free, 1 = occupied
int fileCount = 0;

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

    // Find the starting block
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
            // Free the blocks
            for (int j = 0; j < fileTable[i].blocksCount; j++) {
                blockStatus[fileTable[i].blocks[j]] = 0;
            }
            // Shift the files in the table
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

void read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[100];
    char command[20], name[50];
    int size, offset;
    char data[100];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue; // Ignore comments

        if (sscanf(line, "CREATE %s %d", name, &size) == 2) {
            create_file(name, size);
        } else if (sscanf(line, "DELETE %s", name) == 1) {
            delete_file(name);
        } else if (sscanf(line, "WRITE %s %d %[^\n]", name, &offset, data) == 3) {
            write_file(name, offset, data);
        } else if (sscanf(line, "READ %s %d %d", name, &offset, &size) == 3) {
            read_file_content(name, offset, size);
        } else if (strncmp(line, "LIST", 4) == 0) {
            list_files();
        } else {
            printf("Error: Unknown command in line: %s", line);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    read_file(argv[1]);
    return 0;
}
