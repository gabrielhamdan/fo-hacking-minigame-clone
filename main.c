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

const char *garbage_char_list = {"!@#$%&*()-_=+[{~^\\|,<.>;:/?}]"};
const char *rand_wrd = {"fallout"};

int get_random_int(int max_rand) {
    time_t t;
    srand((unsigned) time(&t));
    return rand() % max_rand;
}

char * print_mem_dump() {
    for(int i = 0; i < MEM_OFFSET; i++) {
        
    }
}

void print_display() {
    int first_mem_add = get_random_int(MAX_RAND - MIN_RAND);

    for(int i = 0; i < COLUMN_LEN; i++) {
        printf("0x%02X  012345678910\t", first_mem_add);
        printf("0x%02X  012345678910\n", (first_mem_add + COLUMN_LEN * MEM_OFFSET));
        first_mem_add += MEM_OFFSET;
    }
}

int main(int argc, char **argv) {
    print_display();

    return 0;
}