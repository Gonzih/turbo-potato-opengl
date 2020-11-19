#pragma once

#include "logging.hpp"
#include "geometry.hpp"
#include "sdl/sdl.hpp"
#include "ecs/ecs.hpp"
#include "components/components.hpp"
#include "map/map.hpp"

using namespace ecs;
using namespace ecs::components;

#define LIGHT_RADIUS 15

class Game
{
private:
    int m_difficulty = 0;
    bool m_is_running = true;
    int m_screen_width;
    int m_screen_height;
    std::shared_ptr<sdl::Window> m_window;
    System m_system;
    std::shared_ptr<Group> m_tiles_group;
    std::shared_ptr<Group> m_player_group;
    std::shared_ptr<Group> m_enemies_group;
    std::shared_ptr<Group> m_darkness_group;
    std::shared_ptr<MovementComponent> m_player_mvm_cmp;
    std::shared_ptr<TransformComponent> m_player_pos_cmp;
    std::unique_ptr<sdl::SpriteManager> m_sprite_manager;
    std::unique_ptr<Map> m_level;
    std::unique_ptr<LightMap> m_light_map;

public:
    Game(int screen_width, int screen_height) :
        m_screen_width { screen_width },
        m_screen_height { screen_height },
        m_window { std::make_shared<sdl::Window>(screen_width, screen_height) },
        m_sprite_manager { std::make_unique<sdl::SpriteManager>(m_window) }
    { };

    void init()
    {
        m_window->set_resizable(false);

        static int sprite_size = 32;
        m_sprite_manager->preload_sprite("sprites/surroundings.png", 1, 3, sprite_size, sprite_size);
        m_sprite_manager->preload_sprite("sprites/darkness.png", 1, 1, sprite_size, sprite_size);
        m_sprite_manager->preload_sprite("sprites/mage.png", 1, 1, sprite_size, sprite_size, sdl::RGB { 0xFF, 0, 0xFF });

        m_tiles_group = m_system.add_group();
        m_player_group = m_system.add_group();
        auto player = m_player_group->add_entity();

        static int map_width = m_screen_width/sprite_size;
        static int map_height = m_screen_height/sprite_size;

        auto wall_sprite = m_sprite_manager->get_sprite("sprites/surroundings.png");
        logger::info("Loading levels sprite");
        m_level = std::make_unique<Map>(map_width, map_height);
        generate_tiles();

        auto pos =  m_level->get_random_empty_coords();
        logger::info("Initializing player at (x, y)", pos.x, pos.y);

        auto player_sprite = m_sprite_manager->get_sprite("sprites/mage.png");

        player->add_component<TransformComponent>(pos);
        player->add_component<MovementComponent>(get_can_move_fn());
        player->add_component<SpriteComponent>(m_window, player_sprite);
        player->add_component<SpriteRenderComponent>(get_visible_fn());
        m_player_mvm_cmp = player->get_component<MovementComponent>();
        m_player_pos_cmp = player->get_component<TransformComponent>();

        regen_light_map();

        m_enemies_group = m_system.add_group();
        init_enemies();

        m_darkness_group = m_system.add_group();
        auto darkness = m_darkness_group->add_entity();

        auto darkness_sprite = m_sprite_manager->get_sprite("sprites/darkness.png");
        darkness_sprite->set_blend_mode(SDL_BLENDMODE_BLEND);

        darkness->add_component<SpriteComponent>(m_window, darkness_sprite);
        darkness->add_component<DarknessComponent>(map_width, map_height, get_visible_fn(), get_memoized_fn());
    }

    VisibleLambda get_visible_fn()
    {
        return [this](int x, int y)
        {
            return this->visible(x, y);
        };
    }

    CanMoveLambda get_can_move_fn()
    {
        return [this](Vector2D pos, MovementDirection dir)
        {
            return this->can_move(pos, dir);
        };
    }

    MemoizedLambda get_memoized_fn() {
        return [this](int x, int y)
        {
            return this->memoized(x, y);
        };
    }

    void init_enemies()
    {
        auto player_sprite = m_sprite_manager->get_sprite("sprites/mage.png");

        int n = rand_int(4, 11) + m_difficulty;
        for (int i = 0; i < n; ++i) {
            auto enemy = m_enemies_group->add_entity();
            auto pos =  m_level->get_random_empty_coords();
            logger::info("Initializing enemy at (x, y)", pos.x, pos.y);

            enemy->add_component<TransformComponent>(pos);
            enemy->add_component<MovementComponent>(get_can_move_fn());
            enemy->add_component<SpriteComponent>(m_window, player_sprite);
            enemy->add_component<SpriteRenderComponent>(get_visible_fn());
        }
    }

    void quit()
    {
        exit(0);
    }

    void attempt_to_go_next_level()
    {
        auto pos = m_player_pos_cmp->get_pos();
        if (can_go_downstairs(pos))
        {
            go_down_level();
            m_enemies_group->destroy_all();
            m_system.collect_garbage();
            init_enemies();
            m_system.update();
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
            case SDLK_ESCAPE:
                m_is_running = false;
                logger::info("exiting");
                break;
            default:
                logger::info("UNHANDLED KEY", event.key.keysym.sym);
                break;
        }

        if (direction == MovementDirection::None)
            return;

        m_player_mvm_cmp->move(direction);
        regen_light_map();
        m_system.update();

        direction = MovementDirection::None;
    }

    void loop()
    {
        SDL_Event event;
        while(m_is_running)
        {
            m_system.collect_garbage();
            m_window->reset_viewport();
            m_window->clear();

            m_system.draw();
            m_window->update();

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

    // LEVELS RELATED TOOLING

    bool can_move(Vector2D pos, MovementDirection direction) const
    {
        return m_level->can_move(pos, direction);
    }

    bool can_go_downstairs(Vector2D pos) const
    {
        return m_level->at(pos.x, pos.y) == TileType::StairsDown;
    }

    bool visible(int x, int y)
    {
        bool vis = m_light_map->visible(x, y);

        if (vis) { m_level->memoize(x, y); }

        return vis;
    }

    bool memoized(int x, int y)
    {
        return m_level->memoized(x, y);
    }

    void regen_light_map()
    {
        auto pos = m_player_pos_cmp->get_pos();
        m_light_map = m_level->generate_light_map(pos, LIGHT_RADIUS);
    }

    void go_down_level()
    {
        /* add_map(); */

        /* auto pos = get_random_empty_coords(); */
        /* logger::info("Initializing player at (x, y)", pos.x, pos.y); */
        /* set_pos_fn(pos); */
    }

    void generate_tiles()
    {
        auto sprite = m_sprite_manager->get_sprite("sprites/surroundings.png");

        int sprite_col;
        for (int x = 0; x < m_level->get_w(); ++x)
        {
            for (int y = 0; y < m_level->get_h(); ++y)
            {
                auto tile = m_level->at(x, y);
                auto entity = m_tiles_group->add_entity();

                switch (tile)
                {
                    case TileType::Wall:
                        sprite_col = 0;
                        break;
                    case TileType::Empty:
                        sprite_col = 1;
                        break;
                    case TileType::StairsDown:
                        sprite_col = 2;
                        break;
                    case TileType::StairsUp:
                        break;
                }

                entity->add_component<TransformComponent>(Vector2D { x, y });
                entity->add_component<MovementComponent>(get_can_move_fn());
                entity->add_component<SpriteComponent>(m_window, sprite);
                entity->add_component<SpriteRenderComponent>(sprite_col, 0, get_visible_fn());
            }
        }
    }
};
