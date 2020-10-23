#pragma once

#include <ncurses.h>
/* #include <chrono> */

#include "window.hpp"
#include "logging.hpp"
#include "geometry.hpp"
#include "ecs/components.hpp"

#define PLAYER_CHARACTER L"🥔"
#define RAT_CHARACTER L"🐀"

using namespace ecs;
using namespace ecs::components;

class Game
{
private:
    int screen_w, screen_h;
    std::shared_ptr<Window> main_win;
    std::shared_ptr<LevelsComponent> levels_cmp;
    std::shared_ptr<MovementComponent> player_mvm_cmp;
    std::shared_ptr<PositionComponent> player_pos_cmp;
    System system;

public:
    explicit Game(int screen_w, int screen_h) :
        screen_w { screen_w },
        screen_h { screen_h },
        main_win { std::make_shared<Window>(screen_w, screen_h, 0, 0) },
        system { }
    { };

    void init()
    {
        auto levels = system.add_entity();
        auto player = system.add_entity();

        std::weak_ptr<Entity> pptr = player;
        auto get_pos_fn = [pptr] {
            if (auto p = pptr.lock()) {
                return p->get_component<PositionComponent>()->get_pos();
            } else {
                throw std::runtime_error("Could not lock player pointer for get_pos lambda");
            }
        };
        auto set_pos_fn = [pptr](Point pos) {
            if (auto p = pptr.lock()) {
                return p->get_component<PositionComponent>()->set_pos(pos);
            } else {
                throw std::runtime_error("Could not lock player pointer for get_pos lambda");
            }
        };

        levels->add_component<LevelsComponent>(main_win, screen_w, screen_h, get_pos_fn, set_pos_fn);
        levels_cmp = levels->get_component<LevelsComponent>();

        std::weak_ptr<LevelsComponent> lptr = levels_cmp;
        auto can_move_fn = [lptr](Point pos, MovementDirection dir) {
            if (auto l = lptr.lock()) {
                return l->can_move(pos, dir);
            } else {
                throw std::runtime_error("Could not lock levels component pointer in can_move lambda");
            }
        };
        auto visible_fn = [lptr](int x, int y) {
            if (auto l = lptr.lock()) {
                return l->visible(x, y);
            } else {
                throw std::runtime_error("Could not lock levels component pointer in can_move lambda");
            }
        };

        auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
        logger::info("Initializing player at (x, y)", pos.x, pos.y);
        player->add_component<PositionComponent>(pos);
        player->add_component<MovementComponent>(can_move_fn);
        player->add_component<AsciiRenderComponent>(PLAYER_CHARACTER, main_win, visible_fn, false);
        player_mvm_cmp = player->get_component<MovementComponent>();
        player_pos_cmp = player->get_component<PositionComponent>();

        levels->get_component<LevelsComponent>()->regen_light_map();


        init_enemies(levels, can_move_fn, visible_fn);
    }

    void init_enemies(std::shared_ptr<Entity> levels, CanMoveLambda can_move_fn, VisibleLambda visible_fn)
    {
        int n = rand_int(3, 8);
        for (int i = 0; i < n; ++i) {
            auto enemy = system.add_entity();
            auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
            logger::info("Initializing enemy at (x, y)", pos.x, pos.y);

            enemy->add_component<PositionComponent>(pos);
            enemy->add_component<MovementComponent>(can_move_fn);
            enemy->add_component<AsciiRenderComponent>(RAT_CHARACTER, main_win, visible_fn);
        }
    }

    void regen_map()
    {
        levels_cmp->regen_current_map();
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

            player_mvm_cmp->move(direction);
            system.update();
            direction = MovementDirection::None;
        }
    }
};
