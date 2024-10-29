// src/main.c
#include <stdio.h>
#include "file_parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    read_command_file(argv[1]);
    return 0;
}