#include <stdio.h>
#include <iostream>
#include <ncurses.h>

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
}

int main() {
    init_curses();

    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    auto game = Game(nw, nh);

    game.loop();

    exit(0);
    return 0;
}
