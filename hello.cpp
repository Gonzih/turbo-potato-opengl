#include <iostream>
#include <stdio.h>
#include <ncurses.h>

using namespace std;

int main() {
    cout << "hello" << endl;
    switch(getch()) {
        case 65:
            cout << endl << "Up" << endl;//key up
            break;
        case 66:
            cout << endl << "Down" << endl;   // key down
            break;
        case 67:
            cout << endl << "Right" << endl;  // key right
            break;
        case 68:
            cout << endl << "Left" << endl;  // key left
            break;
    }

    return 0;
}
