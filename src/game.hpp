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
    std::shared_ptr<Group> levels_group;
    std::shared_ptr<Group> player_group;
    std::shared_ptr<Group> enemies_group;
    std::shared_ptr<Group> darkness_group;
    std::shared_ptr<LevelsComponent> levels_cmp;
    std::shared_ptr<MovementComponent> player_mvm_cmp;
    std::shared_ptr<PositionComponent> player_pos_cmp;
    std::unique_ptr<sdl::SpriteManager> sprite_manager;

public:
    Game(int screen_width, int screen_height) :
        screen_width { screen_width },
        screen_height { screen_height },
        window { std::make_shared<sdl::Window>(screen_width, screen_height) },
        sprite_manager { std::make_unique<sdl::SpriteManager>(window) }
    { };

    void init()
    {
        static int sprite_size = 32;
        sprite_manager->preload_sprite("sprites/surroundings.png", 1, 3, sprite_size, sprite_size);
        sprite_manager->preload_sprite("sprites/darkness.png", 1, 1, sprite_size, sprite_size);
        sprite_manager->preload_sprite("sprites/mage.png", 1, 1, sprite_size, sprite_size, sdl::RGB { 0xFF, 0, 0xFF });

        levels_group = system.add_group();
        auto levels = levels_group->add_entity();

        player_group = system.add_group();
        auto player = player_group->add_entity();

        auto get_pos_fn = [player] {
            return player->get_component<PositionComponent>()->get_pos();
        };
        auto set_pos_fn = [player](Vector2D pos) {
            return player->get_component<PositionComponent>()->set_pos(pos);
        };

        static int map_width = screen_width/sprite_size;
        static int map_height = screen_height/sprite_size;

        auto wall_sprite = sprite_manager->get_sprite("sprites/surroundings.png");
        logger::info("Loading levels sprite");
        levels->add_component<SpriteComponent>(window, wall_sprite);
        levels->add_component<LevelsComponent>(map_width, map_height, get_pos_fn, set_pos_fn);
        levels_cmp = levels->get_component<LevelsComponent>();

        auto l_cmp = levels_cmp;
        auto can_move_fn = [l_cmp](Vector2D pos, MovementDirection dir) {
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

        auto player_sprite = sprite_manager->get_sprite("sprites/mage.png");

        player->add_component<PositionComponent>(pos);
        player->add_component<MovementComponent>(can_move_fn);
        player->add_component<SpriteComponent>(window, player_sprite);
        player->add_component<SpriteRenderComponent>(visible_fn);
        player_mvm_cmp = player->get_component<MovementComponent>();
        player_pos_cmp = player->get_component<PositionComponent>();

        levels->get_component<LevelsComponent>()->regen_light_map();

        enemies_group = system.add_group();
        init_enemies();

        darkness_group = system.add_group();
        auto darkness = darkness_group->add_entity();

        auto darkness_sprite = sprite_manager->get_sprite("sprites/darkness.png");
        darkness_sprite->set_blend_mode(SDL_BLENDMODE_BLEND);

        darkness->add_component<SpriteComponent>(window, darkness_sprite);
        darkness->add_component<DarknessComponent>(map_width, map_height, visible_fn, memoized_fn);
    }

    void init_enemies()
    {
        auto player_sprite = sprite_manager->get_sprite("sprites/mage.png");

        auto l_cmp = levels_cmp;
        auto can_move_fn = [l_cmp](Vector2D pos, MovementDirection dir) {
            return l_cmp->can_move(pos, dir);
        };
        auto visible_fn = [l_cmp](int x, int y) {
            return l_cmp->visible(x, y);
        };

        int n = rand_int(4, 11) + levels_cmp->get_difficulty();
        for (int i = 0; i < n; ++i) {
            auto enemy = enemies_group->add_entity();
            auto pos =  levels_cmp->get_random_empty_coords();
            logger::info("Initializing enemy at (x, y)", pos.x, pos.y);

            enemy->add_component<PositionComponent>(pos);
            enemy->add_component<MovementComponent>(can_move_fn);
            enemy->add_component<SpriteComponent>(window, player_sprite);
            enemy->add_component<SpriteRenderComponent>(visible_fn);
        }
    }

    void quit()
    {
        exit(0);
    }

    void attempt_to_go_next_level()
    {
        auto pos = player_pos_cmp->get_pos();
        if (levels_cmp->can_go_downstairs(pos))
        {
            levels_cmp->go_down_level();
            enemies_group->destroy_all();
            system.collect_garbage();
            init_enemies();
            system.update();
        }
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
            case SDLK_PERIOD:
                attempt_to_go_next_level();
                logger::info("KEY DOWNSTIARS");
                break;
            default:
                logger::info("UNHANDLED KEY", event.key.keysym.sym);
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
                        break;
                }
            }
        }
    }
};
