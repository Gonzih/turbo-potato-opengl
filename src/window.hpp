#pragma once

#include <SDL.h>

class Window {
    private:
        //The window we'll be rendering to
        /* int width; */
        /* int height; */
        SDL_Window* window = NULL;
        //The surface contained by the window
        SDL_Surface* screenSurface = NULL;

    public:
        Window(int w, int h)
        /* : width { w }, height { h }, */
        {
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

            if (window == NULL) {
                throw std::runtime_error(strcat(strdup("Window could not be created! SDL_Error: "), SDL_GetError()));
            }

            screenSurface = SDL_GetWindowSurface(window);
        };

        void render() {
            //Fill the surface white
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0xFF, 0));
            //Update the surface
            SDL_UpdateWindowSurface(window);
            //Wait two seconds
            SDL_Delay(2000);
        }

        virtual ~Window() {
            //Destroy window
            SDL_DestroyWindow(window);
        };
};
