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
    static int frame = 0;
    ++frame;

    int f = frame / 5;
    if (f == 4) {
        frame = 0;
        f = 0;
    }
    auto sprite = window.load_sprite("foo.png", 1, 4, 64, 205, sdl::RGB { 0, 0xFF, 0xFF });
    sprite.render(window.get_renderer(), f, 0, 120, 120);
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
