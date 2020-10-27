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
    auto bg = window.load_texture("background.png");
    window.render_copy(bg);

    auto foo = window.load_texture("foo.png", 0, 0xFF, 0xFF);
    window.set_viewport(SCREEN_WIDTH/2 - foo.get_w()/2, SCREEN_HEIGHT/2  - foo.get_h()/2, foo.get_w(), foo.get_h());
    window.render_copy(foo);
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
