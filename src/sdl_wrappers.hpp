#pragma once

#include <SDL.h>
#include <SDL_image.h>

namespace sdl
{

    void init_sdl()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error(strcat(strdup("SDL could not initialize! SDL_Error: "), SDL_GetError()));
        }
    }

    void init_image()
    {
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            throw std::runtime_error(strcat(strdup("SDL IMAGE could not be initialize! IMG_Error: "), IMG_GetError()));
        }
    }

    void init()
    {
        init_sdl();
        init_image();
    }

    class Surface {
        private:
            SDL_Surface* surface = NULL;
            bool auto_free = true;
        public:
            Surface() {};
            explicit Surface(SDL_Surface* surface): surface { surface } {};
            explicit Surface(SDL_Window* window): surface { SDL_GetWindowSurface(window) }, auto_free { false }
            {
                if (surface == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Could not get surface from a window : "), SDL_GetError()));
                }
            };
            explicit Surface(std::string path): surface { IMG_Load(path.c_str()) }
            {
                if (surface == NULL)
                {
                    throw std::runtime_error(strcat(strcat(strdup("Could not load surface from a file: "), path.c_str()), SDL_GetError()));
                }
            };

            virtual ~Surface()
            {
                if (surface != NULL && auto_free)
                {
                    SDL_FreeSurface(surface);
                }
            }

            SDL_Surface* operator->() { return surface; }
            SDL_Surface& operator*()  { return *(surface); }
            operator SDL_Surface*()   { return surface; }

            int blit(Surface& source)
            {
                return SDL_BlitSurface(source, NULL, surface, NULL);
            }

            Surface optimize(SDL_PixelFormat* format)
            {
                return Surface { SDL_ConvertSurface(surface, format, 0) };
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

                if (window == NULL)
                {
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
                Surface img { Surface("hello_world.bmp").optimize(screenSurface->format) };
                screenSurface.blit(img);
                /* SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) ); */
                update();
            }

            virtual ~Window()
            {
                if (window != NULL)
                {
                    SDL_DestroyWindow(window);
                }
            };
    };
}
