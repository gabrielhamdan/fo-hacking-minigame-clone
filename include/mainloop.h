#ifndef _MAIN_LOOP_H
#define _MAIN_LOOP_H

#include "../include/includes.h"

#define MIN_RAND 0xf001
#define MAX_RAND 0xffff
#define MEM_OFFSET 12
#define COLUMN_LEN 16
#define MEM_DUMP_LEN MEM_OFFSET * COLUMN_LEN * 2
#define GUESS_LEN 16
#define DICT_FILE_PATH "./data/DICT0%d"

typedef enum game_difficulty {
    EASY, MEDIUM, HARD
} game_difficulty;

typedef struct input_buffer {
    char buff[16][16];
    int entries;
} input_buffer;

typedef struct game_params {
    game_difficulty difficulty;
    int dict_size;
    int wrd_len;
    char **dict;
    int words_displayed[32];
    int words_displayed_len;
    char  *secret;
    char guess[GUESS_LEN];
    int attempts_left;
    int first_mem_add;
    input_buffer buff;
} game_params;

void clean__entry_buff(input_buffer *);
void clear_game(game_params *);
void gen_random_garbage_stream();
bool is_correct_guess(game_params *);
void populate_garbage_stream(game_params *);
void print_attempt_failure(game_params *);
void print_display(game_params *);
void print_game_over(bool);
char * print_garbage_stream(int);
void print_left_attempts(int);
void print_terminal_header(int);
void read_usr_input(char *);
void set_difficulty(game_params *);
void set_new_rand_mem_add(game_params *);
void set_new_secret(game_params *);
void start_new_game(bool, game_difficulty);

#endif