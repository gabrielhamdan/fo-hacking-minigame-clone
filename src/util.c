#include "../include/util.h"

void abort_on_null_ptr(void *ptr) {
    if(ptr) return;

    printf("ERROR: failed to allocate memory.\n");
    exit(EXIT_FAILURE);
}

int get_random_int(int max_rand) {
    return rand() % max_rand;
}

void shuffle(char **list, int size) {
    char *aux = malloc(sizeof(char) * strlen(list[0]));
    abort_on_null_ptr(aux);
    for(int i = 0; i < size; i++) {
        int rand_index = get_random_int(size);
        strcpy(aux, list[i]);
        strcpy(list[i], list[rand_index]);
        strcpy(list[rand_index], aux);
    }
    free(aux);
}

void clean_input(char *str) {
    for(int i = 0; i < strlen(str); i++)
        if(str[i] == '\n')
            str[i] = '\0';
}

void str_toupper(char *str) {
    for(int i = 0; i < strlen(str); i++)
        str[i] = toupper(str[i]);
}

bool str_contains_char(char *str, char c) {
    for(int i = 0; i < strlen(str); i++)
        if(str[i] == c)
            return true;
    return false;
}

void flush_stdin() {
    char c;
    while ((c = getc(stdin)) != '\n' && c != EOF);
}