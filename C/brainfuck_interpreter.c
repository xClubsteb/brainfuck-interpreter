#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void brainfuck(void);
char* get_bf_commands(void);
long find_file_size(void);
long find_string_size(void);

int main() {
    brainfuck();
    return 0;
}

void brainfuck(void) {
    uint8_t memory[30000] = {0U};
    uint8_t* memPtr = &memory[0];
    size_t strpos = 0;
    long memoryPos = 0;

    char* brainfuck_string = get_bf_commands(); //dont forget to free it myself in the future!!!
    size_t str_length = strlen(brainfuck_string);

    while (strpos < str_length) {
        char cmd = brainfuck_string[strpos];

        switch (cmd) {
            case '+':
                (*memPtr)++;
                break;
            case '-':
                (*memPtr)--;
                break;
            case '>':
                memPtr++;
                memoryPos++;
                if (memoryPos > 29999) {
                    printf("Error:  memPtr out of bounds");
                    free(brainfuck_string);
                    exit(1);
                }
                break;
            case '<':
                memPtr--;
                memoryPos--;
                if (memoryPos < 0) {
                    printf("Error: memPtr out of bounds");
                    free(brainfuck_string);
                    exit(1);
                }
                break;
            case ',':
                (*memPtr) = getchar();
                break;
            case '.':
                int high_ascii = 126;
                int low_ascii = 32;
                if (*memPtr >= low_ascii && *memPtr <= high_ascii) {
                    putchar(*memPtr);
                }
                else {
                    printf("output: %02X", *memPtr);
                }
                break;
            case '[':
                if (*memPtr == 0) {   // skip loop if current cell is zero
                    int loop = 1;
                    while (loop > 0) {
                        strpos++;
                        if (strpos >= str_length) {
                            printf("Error: unmatched [\n");
                            free(brainfuck_string);
                            exit(1);
                        }
                        if (brainfuck_string[strpos] == '[') loop++;
                        else if (brainfuck_string[strpos] == ']') loop--;
                    }
                }
                break;

            case ']':
                if (*memPtr != 0) {   // jump back if current cell is non-zero
                    int loop = 1;
                    while (loop > 0) {
                        if (strpos == 0) {
                            printf("Error: unmatched ]\n");
                            free(brainfuck_string);
                            exit(1);
                        }
                        strpos--;
                        if (brainfuck_string[strpos] == ']') loop++;
                        else if (brainfuck_string[strpos] == '[') loop--;
                    }
                }
                break;
        }
        strpos++;
    }
    free(brainfuck_string);

}

char* get_bf_commands(void){
    long file_size = find_file_size();
    long string_size = find_string_size();

    if (!(file_size != -1 && string_size != -1)){
        return NULL;
    }

    printf("size: %li\n", file_size);
    printf("string size: %li\n", string_size);

    char* string = malloc(sizeof(char) * (string_size + 1));
    if (string == NULL) {
        return NULL;
    }

    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("File Not Found!\n");
        free(string);
        return NULL;
    }

    const char cmds[8] = {'>', '<', '+', '-', '.', ',', '[', ']'};
    int ch;
    size_t str_ptr = 0;
    while((ch = fgetc(fp)) != EOF) {
        for (int i = 0; i<8; i++) {
            if (ch == cmds[i]) {
                string[str_ptr++] = ch;
                break;
            }
        }
    }
    string[str_ptr] = '\0';
    fclose(fp);
    return string;    
}

long find_file_size(void) {
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long res = ftell(fp);
    fclose(fp);

    return res;
}

long find_string_size(void) {
    FILE *fp = fopen("input.txt", "r");
    long string_length = 0;
    const char cmds[8] = {'>', '<', '+', '-', '.', ',', '[', ']'};

    if (fp == NULL) {
        perror("Cant open file");
        return -1;
    }
    
    int ch;
    while((ch = fgetc(fp)) != EOF) {
        for (int i = 0; i < 8; i++) {
            if (ch == cmds[i]){
                string_length++;
                break;
            }
        }
    }

    fclose(fp);
    return string_length;
}