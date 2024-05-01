#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MIN_RAND 0xf001
#define MAX_RAND 0xffff
#define MEM_OFFSET 12
#define COLUMN_LEN 16
#define MEM_DUMP_LEN MEM_OFFSET * COLUMN_LEN * 2
#define F_WORDS_LEN 80

const char *garbage_char_list = {"!@#$%&*()-_=+[{~^\\|,<.>;:/?}]"};
char mem_dump[MEM_DUMP_LEN];
char mem_dump_chunk[MEM_OFFSET];

typedef enum game_difficulty {
    EASY, MEDIUM, HARD
} game_difficulty;

typedef struct game_params {
    game_difficulty difficulty;
    int dict_size;
    int wrd_len;
    char **dict;
    int words_displayed[32];
    char  *secret;
} game_params;

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

void populate_garbage_stream(game_params *game) {
    // shuffle wrds_list
    int wrd_first_char = get_random_int(32);
    int i = 0;
    while(wrd_first_char < strlen(mem_dump) - game->wrd_len - 1) {
        int rand_wrd = get_random_int(game->dict_size);
        game->words_displayed[i++] = rand_wrd;
        int j;
        for(j = 0; j < game->wrd_len - 1; j++)
            mem_dump[wrd_first_char++] = game->dict[rand_wrd][j];

        wrd_first_char = wrd_first_char + j + get_random_int(32);
    }
}

void print_display(game_params *game) {
    int first_mem_add = MAX_RAND & get_random_int(MIN_RAND);
    for(int i = 0; i < COLUMN_LEN; i++) {
        printf("0x%04X  %s\t", first_mem_add, print_garbage_stream(i * MEM_OFFSET));
        printf("0x%04X  %s\n", (first_mem_add + COLUMN_LEN * MEM_OFFSET), print_garbage_stream(i * MEM_OFFSET + COLUMN_LEN * MEM_OFFSET));
        first_mem_add += MEM_OFFSET;
    }
}

void set_difficulty(char *fname) {
    strncpy(fname, ".dict00", strlen(".dict00"));
}

void start_new_game() {
    game_params game;

    char fname[64];
    set_difficulty(fname);
    FILE *f = fopen(fname, "rb");

    if(!f)
        return exit(EXIT_FAILURE);

    fread(&game.dict_size, sizeof(int), 1, f);
    fread(&game.wrd_len, sizeof(int), 1, f);

    game.dict = malloc(sizeof(char *) * game.dict_size);
    for(int i = 0; i < game.dict_size; i++)
        game.dict[i] = malloc(sizeof(char) * game.wrd_len);

    int i = 0;
    while (fread(game.dict[i++], sizeof(char), game.wrd_len, f) > 0);
    fclose(f);
    
    gen_random_garbage_stream();
    populate_garbage_stream(&game);
    // set new secret
    print_display(&game);
    // main loop

    for (int i = 0; i < game.dict_size; i++) {
        free(game.dict[i]);
    }

    free(game.dict);
}

// int create_dict() {
//     char wrds_list[80][5];
//     FILE *f = fopen("", "r");
//     f = fopen("easy", "wb");
//     int x = 80;
//     int y = 5;
//     fwrite(&x, sizeof(int), 1, f);
//     fwrite(&y, sizeof(int), 1, f);
//     fwrite(wrds_list, sizeof(char), 80 * 5, f);
//     fclose(f);
// }

int main(int argc, char **argv) {
    time_t t;
    srand((unsigned) time(&t));

    start_new_game();

    return 0;
}