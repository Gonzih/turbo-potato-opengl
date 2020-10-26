#include <SDL.h>
#include <stdio.h>

#include <iostream>
#include <ncurses.h>
#include <locale.h>

#include "random.hpp"
/* #include "game.hpp" */
#include "sdl_wrappers.hpp"
#include "logging.hpp"
#include "sig.hpp"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
            window.render();

            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                    quit = true;
            }
        }
    }

    SDL_Quit();

    return 0;
}
