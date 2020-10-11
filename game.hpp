#include <ncurses.h>

#include "map.hpp"
#include "window.hpp"

class Game {
    private:
        Map map;
        Window main_win;

    public:
        Game(int screen_w, int screen_h) :
            map(screen_w, screen_h),
            main_win(screen_w, screen_h, 0, 0)
        { };

        void render() {
            main_win.erase();

            for (int i = 0; i < map.get_width(); i++) {
                for (int j = 0; j < map.get_height(); j++) {
                    main_win.render_char(map.at(i, j), i, j);
                }
            }

            main_win.refresh();
        }

        void loop() {
            for(;;) {
                render();
                int c = getch();
                switch(c) {
                    case KEY_UP:
                        main_win.print("Up", 0, 0);
                        break;
                    case KEY_DOWN:
                        main_win.print("Down", 0, 0);
                        break;
                    case KEY_RIGHT:
                        main_win.print("Right", 0, 0);
                        break;
                    case KEY_LEFT:
                        main_win.print("Left", 0, 0);
                        break;
                }
            }
        }
};
