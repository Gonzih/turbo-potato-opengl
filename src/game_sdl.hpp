#pragma once

#include "logging.hpp"
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
    sdl::Window window;
    System system;

public:
    Game(int screen_width, int screen_height) :
        screen_width { screen_width },
        screen_height { screen_height },
        window { screen_width, screen_height }
    { };

    void init()
    {
        logger::info("Starting with screen dims", screen_width, screen_height);
        auto player = system.add_entity();
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

    void render(sdl::Window& window)
    {
        static int frame = 0;
        static int sprite_width = 64;
        static int sprite_height = 205;
        static int pos_x = 120;
        static int pos_y = 120;

        ++pos_x;
        ++frame;
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        int f = frame / 5;
        if (f == 4) {
            frame = 0;
            f = 0;
        }
        auto sprite = window.load_sprite("foo.png", 1, 4, sprite_width, sprite_height, sdl::RGB { 0, 0xFF, 0xFF });
        sprite.render(window.get_renderer(), f, 0, pos_x, pos_y, 0, NULL, flip);
    }

    void loop()
    {
        SDL_Event event;
        while(true)
        {
            window.reset_viewport();
            window.clear();

            render(window);
            window.update();

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
