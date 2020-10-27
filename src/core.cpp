#include <SDL.h>
#include <stdio.h>

#include <iostream>
#include <ncurses.h>
#include <locale.h>

#include "random.hpp"
/* #include "game.hpp" */
#include "sdl/sdl.hpp"
#include "logging.hpp"
#include "sig.hpp"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void render(sdl::Window& window)
{
    auto sprite = window.load_sprite("sprites.png", 2, 2, 100, 100, sdl::RGB { 0, 0xFF, 0xFF });
    /* sprite.set_color_mod(sdl::RGB { 0, 0xFF, 0xFF }); */

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            sprite.render(window.get_renderer(), i, j, 120 * i, 120 * j);
        }
    }
}

int main()
{
    logger::init("turbo-potato.log");
    sigint_handler_init();
    rand_init();

    sdl::init();

    {
        sdl::Window window(SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_Event e;

        bool quit = false;

        while (!quit)
        {
            window.reset_viewport();
            window.clear();

            render(window);

            window.update();

            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                    quit = true;
            }
        }
    }

    sdl::quit();

    return 0;
}
