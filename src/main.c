#include "../include/includes.h"

bool is_debug_mode = DEBUG;

int main(int argc, char **argv) {
    system("clear");

    time_t t;
    srand((unsigned) time(&t));

    print_main_menu(is_debug_mode);

    return 0;
}