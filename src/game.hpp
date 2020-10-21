#pragma once

#include <ncurses.h>
#include <chrono>

#include "map.hpp"
#include "window.hpp"
#include "logging.hpp"
#include "geometry.hpp"
#include "ecs/components.hpp"

#define PLAYER_CHARACTER L"🥔"
#define LIGHT_RADIUS 15

using namespace ecs;
using namespace ecs::components;

class Game
{
private:
    int screen_w, screen_h;
    Map map;
    Window main_win;
    System system;
    std::shared_ptr<Entity> player;

public:
    explicit Game(int screen_w, int screen_h) :
        screen_w { screen_w },
        screen_h { screen_h },
        map { screen_w, screen_h },
        main_win { screen_w, screen_h, 0, 0 },
        system {  },
        player { system.add_entity() }
    { };

    void init()
    {
        init_player();
    }

    void init_player()
    {
        auto pos =  map.get_random_empty_coords();
        log::info("Initializing player at (x, y)", pos.x, pos.y);

        player->add_component<PositionComponent>(pos);
        player->add_component<AsciiRenderComponent>(PLAYER_CHARACTER);
    }

    void regen_map()
    {
        log::info("Regenerating map");

        Map newmap { screen_w, screen_h };
        map = newmap;

        auto pos =  map.get_random_empty_coords();
        log::info("Initializing player at (x, y)", pos.x, pos.y);

        player->get_component<PositionComponent>()->move_to(pos);
    }

    void render()
    {
        system.draw();

        main_win.erase();

        auto player_pos = player->get_component<PositionComponent>()->get_pos();
        auto light_map = map.generate_light_map(player_pos, LIGHT_RADIUS);
        int c;

        for (int i = 0; i < map.get_width(); ++i)
        {
            for (int j = 0; j < map.get_height(); ++j)
            {
                c = map.at(i, j);

                if (!light_map.visible(i, j)) {
                    if (map.memoized(i, j)) {
                        c |= A_DIM;
                    } else {
                        c = '.' | A_DIM;
                    }
                } else {
                    map.memoize(i, j);
                }

                main_win.render_char(c, i, j);
            }
        }

        for (auto& entity : system.get_entities())
        {
            if (entity->has_component<AsciiRenderComponent>() && entity->has_component<AsciiRenderComponent>())
                main_win.print(
                        entity->get_component<AsciiRenderComponent>()->get_symbol(),
                        entity->get_component<PositionComponent>()->get_x(),
                        entity->get_component<PositionComponent>()->get_y());

        }


        main_win.refresh();
    }

    void loop()
    {
        int c;
        MovementDirection direction;
        std::chrono::time_point<std::chrono::system_clock> then = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

        for(;;) {
            now = std::chrono::system_clock::now();
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
            auto ms = milliseconds.count();

            if (ms > 100) {
                system.update();
                then = now;
            }

            system.collect_garbage();
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

            if (map.can_move(player->get_component<PositionComponent>()->get_pos(), direction)) {
                player->get_component<PositionComponent>()->move(direction);
                direction = MovementDirection::None;
            }
        }
    }
};
