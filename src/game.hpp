#pragma once

#include <ncurses.h>
/* #include <chrono> */

#include "window.hpp"
#include "logging.hpp"
#include "geometry.hpp"
#include "ecs/components.hpp"

#define PLAYER_CHARACTER L"🥔"

using namespace ecs;
using namespace ecs::components;

class Game
{
private:
    int screen_w, screen_h;
    std::shared_ptr<Window> main_win;
    System system;
    std::shared_ptr<Entity> levels;
    std::shared_ptr<Entity> player;

public:
    explicit Game(int screen_w, int screen_h) :
        screen_w { screen_w },
        screen_h { screen_h },
        main_win { std::make_shared<Window>(screen_w, screen_h, 0, 0) },
        system {  },
        levels { system.add_entity() },
        player { system.add_entity() }
    { };

    void init()
    {
        init_levels();
        init_player();
        levels->get_component<LevelsComponent>()->regen_light_map();
    }

    void init_levels()
    {
        levels->add_component<LevelsComponent>(player, main_win, screen_w, screen_h);
    }

    void init_player()
    {
        auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
        logger::info("Initializing player at (x, y)", pos.x, pos.y);

        player->add_component<PositionComponent>(pos);
        player->add_component<AsciiRenderComponent>(PLAYER_CHARACTER, main_win);
    }

    void regen_map()
    {
        levels->get_component<LevelsComponent>()->regen_current_map();
    }

    void render()
    {
        main_win->erase();
        system.draw();
        main_win->refresh();
    }

    void loop()
    {
        int c;
        MovementDirection direction;

        for(;;) {
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

            if (levels->get_component<LevelsComponent>()->can_move(player->get_component<PositionComponent>()->get_pos(), direction)) {
                player->get_component<PositionComponent>()->move(direction);
                system.update();
                direction = MovementDirection::None;
            }
        }
    }
};
