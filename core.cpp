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
    nodelay(stdscr, TRUE);
}

int main() {
    init_curses();

    auto game = Game(20, 20);

    game.loop();

    return 0;
}
