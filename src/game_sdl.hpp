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

public:
    Game(int screen_width, int screen_height) :
        screen_width { screen_width },
        screen_height { screen_height },
        window { std::make_shared<sdl::Window>(screen_width, screen_height) }
    { };

    void init()
    {

        logger::info("Starting with screen dims", screen_width, screen_height);
        auto player = system.add_entity();
        player->add_component<PositionComponent>(Point {120, 120});

        // FIXME: not sure why this doesn't work
        // static int sprite_width = 64;
        // static int sprite_height = 205;
        // auto sprite = std::move(window->load_sprite("foo.png", 1, 4, sprite_width, sprite_height, sdl::RGB { 0, 0xFF, 0xFF }));
        // player->add_component<SpriteRenderComponent>(std::move(sprite), window);
        player->add_component<SpriteRenderComponent>(window);
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
