#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<iostream>

using namespace std;

// From https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar-for-reading-a-single-character-only
class DisableInputBufferWrapper {
public:
    DisableInputBufferWrapper() {
        // tcgetattr gets the parameters of the current terminal
        // STDIN_FILENO will tell tcgetattr that it should write the settings
        // of stdin to oldt
        tcgetattr( STDIN_FILENO, &oldt);
        // now the settings will be copied
        newt = oldt;
        // ICANON normally takes care that one line at a time will be processed
        // that means it will return if it sees a "\n" or an EOF or an EOL
        newt.c_lflag &= ~(ICANON);
        // Disassocaite input from output
        newt.c_lflag &= ~(ICANON | ECHO); 
        // Those new settings will be set to STDIN
        // TCSANOW tells tcsetattr to change attributes immediately.
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
    }
    ~DisableInputBufferWrapper() {
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    }
private:
    struct termios oldt, newt;
};

int main() {
    DisableInputBufferWrapper _;
    int c;

    cout << "hello" << endl;

    while ((c= getchar())!= 'e') {
        switch(c) {
            case 65:
                cout  << "Up" << endl;//key up
                break;
            case 66:
                cout  << "Down" << endl;   // key down
                break;
            case 67:
                cout << "Right" << endl;  // key right
                break;
            case 68:
                cout << "Left" << endl;  // key left
                break;
        }
    }

    return 0;
}
