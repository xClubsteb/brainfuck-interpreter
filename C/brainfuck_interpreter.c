#include <stdio.h>
#include <stdlib.h>

void brainfuck(void);
char* get_bf_commands(void);
long find_file_size(void);
long find_string_size(void);

int main() {
    brainfuck();
    return 0;
}

void brainfuck(void) {
    char* brainfuck_string = get_bf_commands();
    printf("input_string: %s\n", brainfuck_string);
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