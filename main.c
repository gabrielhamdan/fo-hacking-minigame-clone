#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MIN_RAND 61440
#define MAX_RAND 65535
#define MEM_OFFSET 12
#define COLUMN_LEN 16
#define MEM_LEN (COLUMN_LEN * 2)
#define GARBAGE_CHAR_LIST_LEN 29
#define MEM_DUMP_LEN MEM_OFFSET * COLUMN_LEN * 2

const char *garbage_char_list = {"!@#$%&*()-_=+[{~^\\|,<.>;:/?}]"};
const char *rand_wrd = {"WORD"};
char mem_dump[MEM_DUMP_LEN];
char mem_dump_chunk[MEM_OFFSET];

int get_random_int(int max_rand) {
    return rand() % max_rand;
}

void gen_random_garbage_stream() {
    for(int i = 0; i < MEM_DUMP_LEN; i++)
        mem_dump[i] = garbage_char_list[get_random_int(strlen(garbage_char_list))];
}

char * print_garbage_stream(int offset) {
    for(int i = offset, j = 0; j < MEM_OFFSET; i++, j++)
        mem_dump_chunk[j] = mem_dump[i];
    return mem_dump_chunk;
}

char * print_mem_dump() {
    int wrd_first_char = get_random_int(MEM_OFFSET - strlen(rand_wrd));
    for(int i = 0; i < strlen(rand_wrd); i++)
        mem_dump[wrd_first_char++] = rand_wrd[i];

    return mem_dump;
}

void populate_garbage_stream() {
    int wrd_first_char = get_random_int(64);
    while(wrd_first_char < strlen(mem_dump) - strlen(rand_wrd)) {
        int j;
        for(j = 0; j < strlen(rand_wrd); j++)
            mem_dump[wrd_first_char++] = rand_wrd[j];

        wrd_first_char = wrd_first_char + j + get_random_int(64);
    }
}

void print_display() {
    int first_mem_add = get_random_int(MAX_RAND - MIN_RAND);
    gen_random_garbage_stream();
    populate_garbage_stream();

    for(int i = 0; i < COLUMN_LEN; i++) {
        printf("0x%04X  %s\t", first_mem_add, print_garbage_stream(i * MEM_OFFSET));
        printf("0x%04X  %s\n", (first_mem_add + COLUMN_LEN * MEM_OFFSET), print_garbage_stream(i * MEM_OFFSET + COLUMN_LEN * MEM_OFFSET));
        first_mem_add += MEM_OFFSET;
    }
}

int main(int argc, char **argv) {
    time_t t;
    srand((unsigned) time(&t));

    print_display();

    return 0;
}