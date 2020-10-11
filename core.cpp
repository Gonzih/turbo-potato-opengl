#include <stdio.h>
#include <iostream>
#include <ncurses.h>

#include "random.hpp"
#include "game.hpp"

/*
 * Game class
 * Map class, generate map via random rectangles
 * Render cycle
 * Player class
 * Render player
 * ???
 * Profit
 */

using namespace std;

void init_curses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main() {
    init_curses();
    rand_init();

    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    auto game = Game(nw, nh);

    game.loop();

    exit(0);
    return 0;
}
