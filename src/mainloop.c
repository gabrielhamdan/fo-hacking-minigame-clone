#include "../include/mainloop.h"

char mem_dump[MEM_DUMP_LEN];
char mem_dump_chunk[MEM_OFFSET];

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

void set_difficulty(game_params * game) {
    char fname[64];
    sprintf(fname, DICT_FILE_PATH, game->difficulty);

    FILE *f = fopen(fname, "rb");
    if(!f)
        return exit(EXIT_FAILURE);

    fread(&(game->dict_size), sizeof(int), 1, f);
    fread(&(game->wrd_len), sizeof(int), 1, f);

    game->dict = malloc(sizeof(char *) * game->dict_size);
    abort_on_null_ptr(game->dict);

    for(int i = 0; i < game->dict_size; i++)
        game->dict[i] = malloc(sizeof(char) * game->wrd_len + 1);

    int i = 0;
    while (fread(game->dict[i++], sizeof(char), game->wrd_len, f) > 0);
    fclose(f);

    switch (game->difficulty) {
        case EASY:
            game->attempts_left = 5;
            break;
        case MEDIUM:
            game->attempts_left = 4;
            break;
        case HARD:
            game->attempts_left = 3;
            break;
    }
}

void set_new_secret(game_params *game) {
    int rand_index = get_random_int(game->words_displayed_len);
    game->secret = malloc(sizeof(char) * game->wrd_len);
    abort_on_null_ptr(game->secret);
    strcpy(game->secret, game->dict[game->words_displayed[rand_index]]);
}

void clear_game(game_params *game) {
    for (int i = 0; i < game->dict_size; i++)
        free(game->dict[i]);
    free(game->dict);
    free(game->secret);
}

bool is_correct_guess(game_params *game) {
    clean_input(game->guess);
    str_toupper(game->guess);
    return strcmp(game->guess, game->secret) == 0;
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

void print_game_over(bool has_guessed) {
    if(has_guessed)
        printf(">Exact match!\n>Please wait while system is accessed.\n>");
    else {
        system("clear");
        printf("TERMINAL LOCKED\nPLEASE CONTACT AN ADMINISTRATOR\n");
    }
}

void read_usr_input(char *guess) {
    printf(">");
    scanf("%7s", guess);
    flush_stdin();
}

void clean__entry_buff(input_buffer *entry_buff) {
    for(int i = 0; i < COLUMN_LEN; i++)
        memset(entry_buff->buff[i], '\0', sizeof(entry_buff->buff[i]));
    entry_buff->entries = 0;
}

void start_new_game(bool is_debug_mode, game_difficulty difficulty) {
    game_params game;
    game.difficulty = difficulty;

    clean__entry_buff(&game.buff);
    set_difficulty(&game);
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
        read_usr_input(&game.guess);
        if(is_correct_guess(&game)) break;
        game.attempts_left--;
        print_attempt_failure(&game);
    }

    print_game_over(game.attempts_left > 0);

    clear_game(&game);

    // wait for user input
    getchar();

    print_main_menu(is_debug_mode);
}