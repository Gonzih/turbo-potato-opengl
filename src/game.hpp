#include <ncurses.h>
/* #include <chrono> */
/* #include <thread> */

#include "map.hpp"
#include "window.hpp"
#include "character.hpp"
#include "logging.hpp"

#define PLAYER_CHARACTER '@' | A_BLINK
#define LIGHT_RADIUS 15

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
            logger->info("Initializing player at ({}, {})", player_pos.first, player_pos.second);
            player.move_to(player_pos);
        }

        void regen_map() {
            logger->info("Regenerating map");
            Map newmap(screen_w, screen_h);
            map = newmap;
            init_player_pos();
        }

        void render() {
            main_win.erase();

            auto light_map = map.generate_light_map(player.get_pos(), LIGHT_RADIUS);
            int c;

            for (int i = 0; i < map.get_width(); i++) {
                for (int j = 0; j < map.get_height(); j++) {
                    c = map.at(i, j);

                    if (!light_map.visible(i, j))
                        c |= A_DIM;

                    main_win.render_char(c, i, j);
                }
            }

            main_win.render_char(PLAYER_CHARACTER, player.get_x(), player.get_y());

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
                    case ' ':
                        regen_map();
                        break;
                }
            }
        }
};
