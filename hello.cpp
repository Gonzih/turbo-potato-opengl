#include <stdio.h>
#include <iostream>
#include <ncurses.h>

#include "window.hpp"

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

int y0, x0, nlines, ncols;

int main() {
    y0 = 10;
    x0 = 10;
    nlines = 2;
    ncols = 8;

    init_curses();

    auto win = Window(ncols, nlines, x0, y0);

    for(;;) {
        int c = getch();
        switch(c) {
            case KEY_UP:
                win.print("Up", 0, 0);
                break;
            case KEY_DOWN:
                win.print("Down", 0, 0);
                break;
            case KEY_RIGHT:
                win.print("Right", 0, 0);
                break;
            case KEY_LEFT:
                win.print("Left", 0, 0);
                break;
        }
    }

    return 0;
}
