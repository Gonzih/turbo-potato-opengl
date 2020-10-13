#include <stdio.h>
#include <iostream>
#include <ncurses.h>

#include "spdlog/spdlog.h"

#include "random.hpp"
#include "game.hpp"
#include "logging.hpp"


using namespace std;

void init_curses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main() {
    logging_init(spdlog::level::info);
    init_curses();
    rand_init();

    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    logger->info("Starting game with width={} height={}", nw, nh);
    auto game = Game(nw, nh);

    game.loop();

    exit(0);
    return 0;
}
