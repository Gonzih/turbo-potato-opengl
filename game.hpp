#include <ncurses.h>

#include "map.hpp"
#include "window.hpp"
#include "character.hpp"

class Game {
    private:
        int screen_w, screen_h;
        Map map;
        Window main_win;
        Player player;

    public:
        Game(int screen_w, int screen_h) :
            screen_w(screen_w),
            screen_h(screen_h),
            map(screen_w, screen_h),
            main_win(screen_w, screen_h, 0, 0),
            player(0, 0)
        {
            init_player_pos();
        };

        void init_player_pos() {
            auto player_pos =  map.get_random_empty_coords();
            player.move_to(player_pos);
        }

        void regen_map() {
            map = Map(screen_w, screen_h);
            init_player_pos();
        }

        void render() {
            main_win.erase();

            for (int i = 0; i < map.get_width(); i++) {
                for (int j = 0; j < map.get_height(); j++) {
                    main_win.render_char(map.at(i, j), i, j);
                }
            }

            main_win.render_char('@', player.get_x(), player.get_y());

            main_win.refresh();
        }

        void loop() {
            for(;;) {
                render();
                int c = getch();
                switch(c) {
                    case KEY_UP:
                        player.move(MovementDirection::Up);
                        break;
                    case KEY_DOWN:
                        player.move(MovementDirection::Down);
                        break;
                    case KEY_RIGHT:
                        player.move(MovementDirection::Right);
                        break;
                    case KEY_LEFT:
                        player.move(MovementDirection::Left);
                        break;
                    case KEY_ENTER:
                        regen_map();
                        break;
                }
            }
        }
};
