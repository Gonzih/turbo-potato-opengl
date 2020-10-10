#include <ncurses.h>

class Window {
    private:
        WINDOW *win;

    public:
        Window(int w, int h, int x0, int y0) {
            win = newwin(h, w, y0, x0);
            box(win, 0, 0);
            wrefresh(win);
        };

        void print(std::string line, int x, int y) {
            wmove(win, x, y);
            wrefresh(win);
            waddstr(win, line.c_str());
        }

        virtual ~Window() {
            delwin(win);
            refresh();
        };
};
