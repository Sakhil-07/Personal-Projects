#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 512
#define MAX_MACROS 100

// Macro structure
typedef struct {
    char key[50];
    char value[100];
} Macro;

Macro macros[MAX_MACROS];
int macro_count = 0;
int in_comment = 0; // Flag to track multi-line comments

// Function prototypes
void process_file(const char *filename);
void remove_comments(char *line);
void replace_macros(char *line);
void process_directives(char *line, FILE *output);

// Function to remove comments
void remove_comments(char *line) {
    char *single_comment = strstr(line, "//");
    char *multi_comment = strstr(line, "/*");
    
    if (single_comment) memset(single_comment, ' ', strlen(single_comment));
    
    if (in_comment) {
        char *end_comment = strstr(line, "*/");
        if (end_comment) {
            memset(line, ' ', end_comment + 2 - line);
            in_comment = 0;
        } else {
            memset(line, ' ', strlen(line));
        }
    }
    
    if (multi_comment) {
        char *end_comment = strstr(multi_comment, "*/");
        if (end_comment) {
            memset(multi_comment, ' ', end_comment + 2 - multi_comment);
        } else {
            memset(multi_comment, ' ', strlen(multi_comment));
            in_comment = 1;
        }
    }
}

// Function to replace macros
void replace_macros(char *line) {
    for (int i = 0; i < macro_count; i++) {
        char *pos;
        while ((pos = strstr(line, macros[i].key))) {
            char temp[MAX_LINE];
            strcpy(temp, pos + strlen(macros[i].key));
            strcpy(pos, macros[i].value);
            strcat(line, temp);
        }
    }
}

// Function to process directives
void process_directives(char *line, FILE *output) {
    if (strncmp(line, "#define", 7) == 0) {
        sscanf(line + 8, "%s %s", macros[macro_count].key, macros[macro_count].value);
        macro_count++;
    } else if (strncmp(line, "#include", 8) == 0) {
        char header_file[50];
        sscanf(line, "#include \"%[^"]\"", header_file);
        process_file(header_file);
    } else {
        replace_macros(line);
        fprintf(output, "%s\n", line);
    }
}

// Function to process a file
void process_file(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }
    
    FILE *output = fopen("processed_output.c", "w");
    if (!output) {
        fprintf(stderr, "Error creating output file\n");
        fclose(input);
        return;
    }
    
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), input)) {
        remove_comments(line);
        process_directives(line, output);
    }
    
    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }
    process_file(argv[1]);
    printf("Preprocessing completed. Output saved to processed_output.c\n");
    return 0;
}

