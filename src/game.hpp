#pragma once

#include "logging.hpp"
#include "geometry.hpp"
#include "sdl/sdl.hpp"
#include "tuvok/tuvok.hpp"
#include "ecs/ecs.hpp"
#include "components/components.hpp"
#include "map/map.hpp"

using namespace ecs;
using namespace ecs::components;

class Game
{
private:
    bool m_is_running = true;
    std::shared_ptr<sdl::Window> m_window;
    /* System m_system; */

public:
    Game(int screen_width, int screen_height) :
        m_window { std::make_shared<sdl::Window>(screen_width, screen_height) }
    { };

    void init()
    {
        m_window->set_resizable(false);
        sdl::setup_opengl();
    }

    void quit()
    {
        exit(0);
    }

    void render()
    {
        sdl::render();
    }


    void handle_keypress(SDL_Event &event)
    {
        static MovementDirection direction;
        static bool shift_pressed = false;
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_LEFT:
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
                        /* attempt_to_go_next_level(); */
                        logger::info("KEY DOWNSTIARS");
                        break;
                    case SDLK_ESCAPE:
                        m_is_running = false;
                        logger::info("exiting");
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift_pressed = true;
                        break;
                    case SDLK_SEMICOLON:
                        if (shift_pressed) {
                            // TODO
                            logger::info("Opening command mode");
                        }
                        break;
                    default:
                        logger::info("UNHANDLED KEY", event.key.keysym.sym);
                        break;
                }
                break; // SDL_KEYDOWN
            case SDL_KEYUP:
                logger::info("KEY RELEASED", event.key.keysym.sym);
                switch(event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                    case SDLK_UP:
                    case SDLK_DOWN:
                        direction = MovementDirection::None;
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift_pressed = false;
                        break;
                }
                break; // SDL_KEYUP
        }

        /* move(direction); */

        direction = MovementDirection::None;
    }

    void loop()
    {
        SDL_Event event;
        SDL_StartTextInput();

        while(m_is_running)
        {
            /* m_system.collect_garbage(); */
            m_window->reset_viewport();
            m_window->clear();

            render();

            /* m_system.draw(); */
            m_window->update();

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
                        break;
                }
            }
        }

        SDL_StopTextInput();
    }
};
