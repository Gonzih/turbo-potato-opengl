#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include <locale.h>

#include "random.hpp"
#include "game.hpp"
#include "logging.hpp"
#include "sig.hpp"

using namespace std;

void curses_init() {
    setlocale(LC_ALL,"");
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main() {
    log::init("turbo-potato.log");
    curses_init();
    sigint_handler_init();
    rand_init();

    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    log::info("Starting game with width height", nw, nh);
    Game game { nw, nh };

    game.loop();

    return 0;
}
