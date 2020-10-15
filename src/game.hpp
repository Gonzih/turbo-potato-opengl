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
        MemoryMap memory_map;

    public:
        Game(int screen_w, int screen_h) :
            screen_w(screen_w),
            screen_h(screen_h),
            map(screen_w, screen_h),
            main_win(screen_w, screen_h, 0, 0),
            player(0, 0),
            memory_map(screen_w, screen_h)
        {
            init_player_pos();
        };

        void init_player_pos() {
            auto player_pos =  map.get_random_empty_coords();
            log::info("Initializing player at (x, y)", player_pos.first, player_pos.second);
            player.move_to(player_pos);
        }

        void regen_map() {
            log::info("Regenerating map");

            Map newmap(screen_w, screen_h);
            map = newmap;

            MemoryMap newmmap(screen_w, screen_h);
            memory_map = newmmap;

            init_player_pos();
        }

        void render() {
            main_win.erase();

            auto light_map = map.generate_light_map(player.get_pos(), LIGHT_RADIUS);
            int c;

            for (int i = 0; i < map.get_width(); i++) {
                for (int j = 0; j < map.get_height(); j++) {
                    c = map.at(i, j);

                    if (!light_map.visible(i, j)) {
                        if (memory_map.memoized(i, j)) {
                            c |= A_DIM;
                        } else {
                            c = '.' | A_DIM;
                        }
                    } else {
                        memory_map.memoize(i, j);
                    }

                    main_win.render_char(c, i, j);
                }
            }

            main_win.render_char(PLAYER_CHARACTER, player.get_x(), player.get_y());

            main_win.refresh();
        }

        void loop() {
            int c;
            MovementDirection direction;

            for(;;) {
                render();
                c = getch();
                switch(c) {
                    case KEY_UP:
                        direction = MovementDirection::Up;
                        break;
                    case KEY_DOWN:
                        direction = MovementDirection::Down;
                        break;
                    case KEY_RIGHT:
                        direction = MovementDirection::Right;
                        break;
                    case KEY_LEFT:
                        direction = MovementDirection::Left;
                        break;
                    case ' ':
                        regen_map();
                        continue;
                };

                if (direction == MovementDirection::None)
                    continue;

                if (map.can_move(player.get_pos(), direction)) {
                    player.move(direction);
                    direction = MovementDirection::None;
                }
            }
        }
};
