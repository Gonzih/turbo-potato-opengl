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
            waddstr(win, line.c_str());
            wrefresh(win);
        }

        void erase() {
            werase(win);
        }

        void refresh() {
            wrefresh(win);
        }

        void render_char(char ch, int x, int y) {
            wmove(win, x, y);
            waddch(win, ch);
            wrefresh(win);
        }

        virtual ~Window() {
            delwin(win);
            refresh();
        };
};
