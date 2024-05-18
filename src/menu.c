#include "../include/menu.h"

void print_main_menu(bool is_debug_mode) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    int selected_index = 0;
    int selected_difficulty = 1;
    game_difficulty difficulty = MEDIUM;
    char selected_option[3][16] = {"NEW GAME", "DIFFICULTY: ", "EXIT"};
    char difficulty_mode[3][16] = {"EASY", "MEDIUM", "HARD"};

    int ch;
    while(1) {
        clear();
        printw("FALLOUT HACKING MINIGAME CLONE\n");
        printw("             by Gabriel Hamdan\n\n");
        for (int i = 0; i < 3; i++) {
            if (i == abs(selected_index))
                printw(" > %s", selected_option[i]);
            else
                printw("   %s", selected_option[i]);
            
            if(i == 1)
                printw("%s\n", difficulty_mode[selected_difficulty]);
            else
                printw("\n");
        }
        refresh();
        switch(ch = getch()) {
            case KEY_UP:
                selected_index = (selected_index - 1 + 3) % 3;
                break;
            case KEY_DOWN:
                selected_index = (selected_index + 1) % 3;
                break;
            case ENTER:
                switch (selected_index) {
                    case NEW_GAME:
                        endwin();
                        start_new_game(is_debug_mode, difficulty);
                        break;
                    case SET_DIFFICULTY:
                        selected_difficulty = (selected_difficulty + 1) % 3;
                        difficulty = selected_difficulty;
                        break;
                    case EXIT:
                        endwin();
                        system("clear");
                        exit(EXIT_SUCCESS);
                        break;
                }
        }
    }
}