#include<stdio.h>
#include<iostream>
#include<ncurses.h>

using namespace std;

int y0, x0, nlines, ncols;

int main() {
    y0 = 0;
    x0 = 0;
    nlines = 20;
    ncols = 80;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear();

    /* WINDOW * win = newwin(nlines, ncols, y0, x0); */

    refresh();

    for(;;) {
        switch(getch()) {
            case KEY_UP:
                cout << endl << "Up" << endl;
                break;
            case KEY_DOWN:
                cout << endl << "Down" << endl;
                break;
            case KEY_RIGHT:
                cout << endl << "Right" << endl;
                break;
            case KEY_LEFT:
                cout << endl << "Left" << endl;
                break;
        }
    }

    return 0;
}
