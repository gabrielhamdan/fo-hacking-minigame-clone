#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MIN_RAND 0xf001
#define MAX_RAND 0xffff
#define MEM_OFFSET 12
#define COLUMN_LEN 16
#define MEM_DUMP_LEN MEM_OFFSET * COLUMN_LEN * 2
#define GUESS_LEN 8
#define ATTEMPS_CHAT "■"

bool is_debug_mode = false;
char mem_dump[MEM_DUMP_LEN];
char mem_dump_chunk[MEM_OFFSET];

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

int get_random_int(int max_rand) {
    return rand() % max_rand;
}

void gen_random_garbage_stream() {
    const char *garbage_char_list = {"!@#$%&*()-_=+[{~^\\|,<.>;:/?}]"};
    for(int i = 0; i < MEM_DUMP_LEN; i++)
        mem_dump[i] = garbage_char_list[get_random_int(strlen(garbage_char_list))];
}

char * print_garbage_stream(int offset) {
    for(int i = offset, j = 0; j < MEM_OFFSET; i++, j++)
        mem_dump_chunk[j] = mem_dump[i];
    return mem_dump_chunk;
}

void shuffle(char **list, int size) {
    char *aux = malloc(sizeof(char) * strlen(list[0]));
    // check malloc
    for(int i = 0; i < size; i++) {
        int rand_index = get_random_int(size);
        strcpy(aux, list[i]);
        strcpy(list[i], list[rand_index]);
        strcpy(list[rand_index], aux);
    }
    free(aux);
}

void populate_garbage_stream(game_params *game) {
    shuffle(game->dict, game->dict_size);
    int wrd_first_char = get_random_int(32);
    int i = 0;
    game->words_displayed_len = 0;
    while(wrd_first_char < strlen(mem_dump) - game->wrd_len) {
        int rand_wrd = get_random_int(game->dict_size);
        game->words_displayed[i++] = rand_wrd;
        game->words_displayed_len++;
        int j;
        for(j = 0; j < game->wrd_len - 1; j++)
            mem_dump[wrd_first_char++] = game->dict[rand_wrd][j];

        wrd_first_char = wrd_first_char + j + get_random_int(32);
    }
}

void print_display(game_params *game) {
    int first_mem_add = game->first_mem_add;
    for(int i = 0; i < COLUMN_LEN; i++) {
        printf("0x%04X  %s\t", first_mem_add, print_garbage_stream(i * MEM_OFFSET));
        printf("0x%04X  %s\t", (first_mem_add + COLUMN_LEN * MEM_OFFSET), print_garbage_stream(i * MEM_OFFSET + COLUMN_LEN * MEM_OFFSET));
        printf("%s\n", game->buff.buff[i]);
        first_mem_add += MEM_OFFSET - 1;
    }
}

void set_difficulty(char *fname) {
    strncpy(fname, ".dict00", strlen(".dict00"));
}

void set_new_secret(game_params *game) {
    int rand_index = get_random_int(game->words_displayed_len);
    game->secret = malloc(sizeof(char) * game->wrd_len);
    // check malloc
    strcpy(game->secret, game->dict[game->words_displayed[rand_index]]);
}

void clear_game(game_params *game) {
    for (int i = 0; i < game->dict_size; i++)
        free(game->dict[i]);
    free(game->dict);
    free(game->secret);
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

bool is_correct_guess(game_params *game) {
    clean_input(game->guess);
    str_toupper(game->guess);
    return strcmp(game->guess, game->secret) == 0;
}

bool str_contains_char(char *str, char c) {
    for(int i = 0; i < strlen(str); i++)
        if(str[i] == c)
            return true;
    return false;
}

void print_attempt_failure(game_params *game) {
    char buff[32];
    sprintf(buff, ">%s", game->guess);
    strcpy(game->buff.buff[game->buff.entries++], buff);
    strcpy(game->buff.buff[game->buff.entries++], ">Entry denied");

    char guesses[GUESS_LEN] = {};
    int match = 0;
    for(int i = 0; i < strlen(game->guess); i++)
        for(int j = 0; j < strlen(game->secret); j++)
            if(game->guess[i] == game->secret[j] && !str_contains_char(guesses, game->guess[i])) {
                guesses[match++] = game->guess[i];
                break;
            }
    
    sprintf(buff, ">%d/%d correct.", match, game->wrd_len - 1);
    strcpy(game->buff.buff[game->buff.entries++], buff);
}

void flush_stdin() {
    char c;
    while ((c = getc(stdin)) != '\n' && c != EOF);
}

void set_new_rand_mem_add(game_params *game) {
    game->first_mem_add = MAX_RAND & get_random_int(MIN_RAND);
}

void print_terminal_header(int attempts_left) {
    printf("ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL\n");
    if(attempts_left > 1)
        printf("ENTER PASSWORD NOW\n\n");
    else
        printf("!!! WARNING: LOCKOUT IMMINENT !!!\n\n");
}

void print_left_attempts(int attempts_left) {
    char buff[64];
    sprintf(buff, "%d ATTEMPT(S) LEFT: ", attempts_left);
    printf("%s", buff);
        
    for(int i = 0, j = strlen(buff); i < attempts_left; i++)
        printf(" ■ ");

    printf("\n\n");
}

void start_new_game(bool is_debug_mode) {
    game_params game;
    game.attempts_left = 4;
    for(int i = 0; i < COLUMN_LEN; i++)
        memset(game.buff.buff[i], '\0', sizeof(game.buff.buff[i]));

    char fname[64];
    set_difficulty(fname);
    FILE *f = fopen(fname, "rb");

    if(!f)
        return exit(EXIT_FAILURE);

    fread(&game.dict_size, sizeof(int), 1, f);
    fread(&game.wrd_len, sizeof(int), 1, f);

    game.dict = malloc(sizeof(char *) * game.dict_size);
    // check mallocs
    for(int i = 0; i < game.dict_size; i++)
        game.dict[i] = malloc(sizeof(char) * game.wrd_len);

    int i = 0;
    while (fread(game.dict[i++], sizeof(char), game.wrd_len, f) > 0);
    fclose(f);

    gen_random_garbage_stream();
    populate_garbage_stream(&game);
    set_new_secret(&game);
    set_new_rand_mem_add(&game);
    
    // main loop
    while(game.attempts_left > 0) {
        system("clear");

        if(is_debug_mode) {
            printf("*** RUNNING ON DEBUG MODE ***\n");
            printf("- secret: %s\n\n", game.secret);
        }

        print_terminal_header(game.attempts_left);
        print_left_attempts(game.attempts_left);
        print_display(&game);
        printf(">");
        fgets(game.guess, GUESS_LEN - 1, stdin);
        if(strlen(game.guess) > game.wrd_len)
            flush_stdin();
        if(is_correct_guess(&game)) break;
        game.attempts_left--;
        print_attempt_failure(&game);
    }

    if(game.attempts_left > 0)
        printf("YOU WIN!\n");
    else
        printf("YOU LOSE!\n");

    clear_game(&game);

    // main menu
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
    if(argc > 1 && strcmp(argv[1], "-d") == 0)
        is_debug_mode = true;

    system("clear");

    time_t t;
    srand((unsigned) time(&t));

    // main menu
    start_new_game(is_debug_mode);

    return 0;
}