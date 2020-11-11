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
        auto darkness = system.add_entity();

        auto get_pos_fn = [player] {
            return player->get_component<PositionComponent>()->get_pos();
        };
        auto set_pos_fn = [player](Point pos) {
            return player->get_component<PositionComponent>()->set_pos(pos);
        };

        static int wall_size = 32;
        static int map_width = screen_width/wall_size;
        static int map_height = screen_height/wall_size;

        std::shared_ptr<sdl::Sprite> wall_sprite = window->load_sprite("sprites/wall.png", 1, 1, wall_size, wall_size);
        logger::info("Loading levels sprite");
        levels->add_component<SpriteComponent>(window, wall_sprite);
        levels->add_component<LevelsComponent>(map_width, map_height, get_pos_fn, set_pos_fn);
        levels_cmp = levels->get_component<LevelsComponent>();

        auto l_cmp = levels_cmp;
        auto can_move_fn = [l_cmp](Point pos, MovementDirection dir) {
            return l_cmp->can_move(pos, dir);
        };
        auto visible_fn = [l_cmp](int x, int y) {
            return l_cmp->visible(x, y);
        };
        auto memoized_fn = [l_cmp](int x, int y) {
            return l_cmp->memoized(x, y);
        };

        auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
        logger::info("Initializing player at (x, y)", pos.x, pos.y);

        static int player_size = 32;
        std::shared_ptr<sdl::Sprite> player_sprite = window->load_sprite("sprites/mage.png", 1, 1, player_size, player_size, sdl::RGB { 0xFF, 0, 0xFF });

        player->add_component<PositionComponent>(pos);
        player->add_component<MovementComponent>(can_move_fn);
        player->add_component<SpriteComponent>(window, player_sprite);
        player->add_component<SpriteRenderComponent>();
        player_mvm_cmp = player->get_component<MovementComponent>();
        player_pos_cmp = player->get_component<PositionComponent>();

        levels->get_component<LevelsComponent>()->regen_light_map();

        init_enemies(levels, can_move_fn, visible_fn);

        static int darkness_size = 32;
        std::shared_ptr<sdl::Sprite> darkness_sprite = window->load_sprite("sprites/darkness.png", 1, 1, darkness_size, darkness_size);
        darkness_sprite->set_blend_mode(SDL_BLENDMODE_BLEND);

        darkness->add_component<SpriteComponent>(window, darkness_sprite);
        darkness->add_component<DarknessComponent>(map_width, map_height, visible_fn, memoized_fn);
    }

    void init_enemies(std::shared_ptr<Entity> levels, CanMoveLambda can_move_fn, VisibleLambda visible_fn)
    {
        static int player_size = 32;
        std::shared_ptr<sdl::Sprite> player_sprite = window->load_sprite("sprites/mage.png", 1, 1, player_size, player_size, sdl::RGB { 0xFF, 0, 0xFF });

        int n = rand_int(3, 8);
        for (int i = 0; i < n; ++i) {
            auto enemy = system.add_entity();
            auto pos =  levels->get_component<LevelsComponent>()->get_random_empty_coords();
            logger::info("Initializing enemy at (x, y)", pos.x, pos.y);

            enemy->add_component<PositionComponent>(pos);
            enemy->add_component<MovementComponent>(can_move_fn);
            enemy->add_component<SpriteComponent>(window, player_sprite);
            enemy->add_component<SpriteRenderComponent>();
        }
    }

    void quit()
    {
        exit(0);
    }

    void handle_keypress(SDL_Event &event)
    {
        static MovementDirection direction;
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT:
                direction = MovementDirection::Left;
                logger::info("KEY LEFT");
                break;
            case SDLK_RIGHT:
                direction = MovementDirection::Right;
                logger::info("KEY RIGHT");
                break;
            case SDLK_UP:
                direction = MovementDirection::Up;
                logger::info("KEY UP");
                break;
            case SDLK_DOWN:
                direction = MovementDirection::Down;
                logger::info("KEY DOWN");
                break;
            default:
                break;
        }

        if (direction == MovementDirection::None)
            return;

        player_mvm_cmp->move(direction);
        system.update();
        direction = MovementDirection::None;
    }

    void loop()
    {
        SDL_Event event;
        while(true)
        {
            system.collect_garbage();
            window->reset_viewport();
            window->clear();

            system.draw();
            window->update();

            while (SDL_PollEvent(&event) != 0)
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                        handle_keypress(event);
                        break;
                    case SDL_KEYUP:
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
