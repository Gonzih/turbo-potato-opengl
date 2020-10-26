#pragma once

#include <SDL.h>

class Surface {
    private:
        SDL_Surface* surface = NULL;
    public:
        Surface() {};
        Surface(SDL_Surface* surface): surface { surface } {};
        Surface(SDL_Window* window): surface { SDL_GetWindowSurface(window) }
        {
            if (surface == NULL) {
                throw std::runtime_error(strcat(strdup("Could not get surface from a window : "), SDL_GetError()));
            }
        };
        Surface(const char* path): surface { SDL_LoadBMP(path) }
        {
            if (surface == NULL) {
                throw std::runtime_error(strcat(strcat(strdup("Could not load surface from a file: "), path), SDL_GetError()));
            }
        };

        virtual ~Surface()
        {
            SDL_FreeSurface(surface);
        }

        SDL_Surface* operator->() { return surface; }
        SDL_Surface& operator*() { return *(surface); }
        operator SDL_Surface*() { return surface; }

        int blit(Surface source)
        {
            return SDL_BlitSurface(source, NULL, surface, NULL);
        }
};

class Window {
    private:
        //The window we'll be rendering to
        /* int width; */
        /* int height; */
        SDL_Window* window = NULL;
        //The surface contained by the window
        Surface screenSurface { };

    public:
        Window(int w, int h)
        /* : width { w }, height { h }, */
        {
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

            if (window == NULL) {
                throw std::runtime_error(strcat(strdup("Window could not be created! SDL_Error: "), SDL_GetError()));
            }

            screenSurface = Surface(window);
        };

        void update()
        {
            SDL_UpdateWindowSurface(window);
        };

        void render()
        {
            Surface img { "snail.bmp" };
            screenSurface.blit(img);
            update();

            SDL_Delay(2000);
        }

        virtual ~Window() {
            //Destroy window
            SDL_DestroyWindow(window);
        };
};
