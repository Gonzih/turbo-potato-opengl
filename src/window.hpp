#pragma once

#include <SDL.h>

class Surface {
    private:
        SDL_Surface* surface = NULL;
        bool auto_free = true;
    public:
        Surface() {};
        Surface(SDL_Surface* surface): surface { surface } {};
        Surface(SDL_Window* window): surface { SDL_GetWindowSurface(window) }, auto_free { false }
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
            if (surface != NULL && auto_free)
                SDL_FreeSurface(surface);
        }

        SDL_Surface* operator->() { return surface; }
        SDL_Surface& operator*() { return *(surface); }
        operator SDL_Surface*() { return surface; }

        int blit(Surface& source)
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
            /* Surface img { "hello_world.bmp" }; */
            /* screenSurface.blit(img); */
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            update();
        }

        virtual ~Window()
        {
            if (window != NULL)
                SDL_DestroyWindow(window);
        };
};
