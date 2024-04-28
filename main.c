#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MIN_RAND 61440
#define MAX_RAND 65535
#define MEM_OFFSET 12
#define COLUMN_LEN 16
#define MEM_LEN (COLUMN_LEN * 2)
#define WORD_LEN 8
#define GARBAGE_CHAR_LIST_LEN 29

const char *garbage_char_list = {"!@#$%&*()-_=+[{~^\\|,<.>;:/?}]"};
const char *rand_wrd = {"word"};
char mem_dump[MEM_OFFSET];

int get_random_int(int max_rand) {
    
    return rand() % max_rand;
}

char * print_mem_dump() {
    int wrd_first_char = get_random_int(MEM_OFFSET - strlen(rand_wrd));
    for(int i = 0; i < MEM_OFFSET; i++)
        mem_dump[i] = garbage_char_list[get_random_int(strlen(garbage_char_list))];

    for(int i = 0; i < strlen(rand_wrd); i++)
        mem_dump[wrd_first_char++] = rand_wrd[i];

    return mem_dump;
}

void print_display() {
    int first_mem_add = get_random_int(MAX_RAND - MIN_RAND);

    for(int i = 0; i < COLUMN_LEN; i++) {
        printf("0x%04X  %s\t", first_mem_add, print_mem_dump());
        printf("0x%04X  %s\n", (first_mem_add + COLUMN_LEN * MEM_OFFSET), print_mem_dump());
        first_mem_add += MEM_OFFSET;
    }
}

int main(int argc, char **argv) {
    time_t t;
    srand((unsigned) time(&t));

    print_display();

    return 0;
}