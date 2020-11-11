#pragma once

#include "logging.hpp"
#include "geometry.hpp"
#include "sdl/sdl.hpp"
#include "ecs/ecs.hpp"
#include "components/components.hpp"

using namespace ecs;
using namespace ecs::components;

class Game
{
private:
    int screen_width;
    int screen_height;
    std::shared_ptr<sdl::Window> window;
    System system;
    std::shared_ptr<LevelsComponent> levels_cmp;
    std::shared_ptr<MovementComponent> player_mvm_cmp;
    std::shared_ptr<PositionComponent> player_pos_cmp;

public:
    Game(int screen_width, int screen_height) :
        screen_width { screen_width },
        screen_height { screen_height },
        window { std::make_shared<sdl::Window>(screen_width, screen_height) }
    { };

    void init()
    {
        auto levels = system.add_entity();
        auto player = system.add_entity();

        auto get_pos_fn = [player] {
            return player->get_component<PositionComponent>()->get_pos();
        };
        auto set_pos_fn = [player](Point pos) {
            return player->get_component<PositionComponent>()->set_pos(pos);
        };

        static int wall_size = 32;
        std::shared_ptr<sdl::Sprite> wall_sprite = window->load_sprite("sprites/wall.png", 1, 1, wall_size, wall_size);
        logger::info("Loading levels sprite");
        levels->add_component<SpriteComponent>(window, wall_sprite);
        levels->add_component<LevelsComponent>(screen_width/wall_size, screen_height/wall_size, wall_size, get_pos_fn, set_pos_fn);
        levels_cmp = levels->get_component<LevelsComponent>();

        auto l_cmp = levels_cmp;
        auto can_move_fn = [l_cmp](Point pos, MovementDirection dir) {
            return l_cmp->can_move(pos, dir);
        };
        auto visible_fn = [l_cmp](int x, int y) {
            return l_cmp->visible(x, y);
        };

        auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
        logger::info("Initializing player at (x, y)", pos.x, pos.y);

        static int sprite_width = 64;
        static int sprite_height = 205;
        std::shared_ptr<sdl::Sprite> player_sprite = window->load_sprite("sprites/foo.png", 1, 4, sprite_width, sprite_height, sdl::RGB { 0, 0xFF, 0xFF });

        player->add_component<PositionComponent>(pos);
        player->add_component<MovementComponent>(can_move_fn);
        player->add_component<SpriteComponent>(window, player_sprite);
        player->add_component<SpriteRenderComponent>();
        player_mvm_cmp = player->get_component<MovementComponent>();
        player_pos_cmp = player->get_component<PositionComponent>();

        levels->get_component<LevelsComponent>()->regen_light_map();
    }

    void quit()
    {
        exit(0);
    }

    void handle_keypress(SDL_Event &event)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT:
                logger::info("KEY LEFT");
                break;
            case SDLK_RIGHT:
                logger::info("KEY RIGHT");
                break;
            case SDLK_UP:
                logger::info("KEY UP");
                break;
            case SDLK_DOWN:
                logger::info("KEY DOWN");
                break;
            default:
                break;
        }
    }

    void loop()
    {
        SDL_Event event;
        while(true)
        {
            window->reset_viewport();
            window->clear();

            system.draw();
            window->update();

            while (SDL_PollEvent(&event) != 0)
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        handle_keypress(event);
                        break;
                    case SDL_QUIT:
                        quit();
                        break;
                    default:
                        logger::info("KEY CAUGHT", event.type);
                        break;
                }
            }
        }
    }
};
