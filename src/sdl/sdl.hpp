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

    void quit()
    {
        IMG_Quit();
        SDL_Quit();
    }

    class Renderer {
        private:
            SDL_Renderer* m_renderer;
        public:
            Renderer() {};
            explicit Renderer(SDL_Window* window)
            : m_renderer { SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) }
            {
                if (m_renderer == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Window Renderer failed to init: "), SDL_GetError()));
                }
            };

            SDL_Renderer* operator->() { return m_renderer; }
            SDL_Renderer& operator*()  { return *(m_renderer); }
            operator SDL_Renderer*()   { return m_renderer; }

            virtual ~Renderer()
            {
                if (m_renderer != NULL)
                { SDL_DestroyRenderer(m_renderer); }
            }
    };

    class Surface {
        private:
            SDL_Surface* m_surface = NULL;
            bool m_auto_free = true;
        public:
            Surface() {};
            explicit Surface(SDL_Surface* surface): m_surface { surface } {};
            explicit Surface(SDL_Window* window): m_surface { SDL_GetWindowSurface(window) }, m_auto_free { false }
            {
                if (m_surface == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Could not get m_surface from a m_window : "), SDL_GetError()));
                }
            };
            explicit Surface(std::string path): m_surface { IMG_Load(path.c_str()) }
            {
                if (m_surface == NULL)
                {
                    throw std::runtime_error(strcat(strcat(strdup("Could not load m_surface from a file: "), path.c_str()), SDL_GetError()));
                }
            };

            virtual ~Surface()
            {
                if (m_surface != NULL && m_auto_free)
                { SDL_FreeSurface(m_surface); }
            }

            SDL_Surface* operator->() { return m_surface; }
            SDL_Surface& operator*()  { return *(m_surface); }
            operator SDL_Surface*()   { return m_surface; }

            int blit(Surface& source)
            {
                return SDL_BlitSurface(source, NULL, m_surface, NULL);
            }

            Surface optimize(SDL_PixelFormat* format)
            {
                return Surface { SDL_ConvertSurface(m_surface, format, 0) };
            }
    };

    class Texture {
        private:
            SDL_Texture* m_texture;
        public:
            Texture() {};
            explicit Texture(std::string path, Renderer& renderer)
            : m_texture { SDL_CreateTextureFromSurface(renderer, Surface { path }) }
            { }

            SDL_Texture* operator->() { return m_texture; }
            SDL_Texture& operator*()  { return *(m_texture); }
            operator SDL_Texture*()   { return m_texture; }

            virtual ~Texture()
            {
                if (m_texture != NULL)
                { SDL_DestroyTexture(m_texture); }
            }
    };

    class Window {
        private:
            SDL_Window* m_window = NULL;
            Renderer m_renderer { };
            /* Surface m_surface { }; */

        public:
            Window(int w, int h)
            : m_window { SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN) }
            {
                if (m_window == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Window could not be created! SDL_Error: "), SDL_GetError()));
                }

                m_renderer = Renderer { m_window };
                /* m_surface  = Surface  { m_window }; */

                SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            };

            void clear()
            {
                SDL_RenderClear(m_renderer);
            };

            void update()
            {
                SDL_RenderPresent(m_renderer);
            };

            void render()
            {
                Texture texture { "preview.png", m_renderer };
                SDL_RenderCopy(m_renderer, texture, NULL, NULL);
            }

            virtual ~Window()
            {
                if (m_window != NULL)
                { SDL_DestroyWindow(m_window); }
            };
    };
}
