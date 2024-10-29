#include <stdio.h>
#include <string.h>
#include "file_system.h"
#include "file_reader.h"

void read_command_file(const char* filename) {
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
