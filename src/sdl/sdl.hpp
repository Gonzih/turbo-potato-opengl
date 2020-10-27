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

        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
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
            explicit Renderer(SDL_Window* window, Uint32 flags)
            : m_renderer { SDL_CreateRenderer(window, -1, flags) }
            {
                if (window == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Cant create renderer from NULL window: "), SDL_GetError()));
                }
                if (m_renderer == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Window Renderer failed to init: "), SDL_GetError()));
                }
            };

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
            int m_width = 0;
            int m_height = 0;

            void set_dimensions()
            {
                m_width = m_surface->w;
                m_height = m_surface->h;
            }
        public:
            Surface() {};
            explicit Surface(SDL_Surface* surface): m_surface { surface } {};
            explicit Surface(SDL_Window* window): m_surface { SDL_GetWindowSurface(window) }, m_auto_free { false }
            {
                if (m_surface == NULL)
                {
                    throw std::runtime_error(strcat(strdup("Could not get m_surface from a m_window : "), SDL_GetError()));
                }
                set_dimensions();
            };
            explicit Surface(std::string path): m_surface { IMG_Load(path.c_str()) }
            {
                if (m_surface == NULL)
                {
                    throw std::runtime_error(strcat(strcat(strdup("Could not load m_surface from a file: "), path.c_str()), SDL_GetError()));
                }
                set_dimensions();
            };

            int get_w() { return m_width; };
            int get_h() { return m_height; };

            virtual ~Surface()
            {
                if (m_surface != NULL && m_auto_free)
                { SDL_FreeSurface(m_surface); }
            }

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

            void set_color_key(int r, int g, int b)
            {
                SDL_SetColorKey(m_surface, SDL_TRUE, SDL_MapRGB(m_surface->format, r, g, b));
            }
    };

    class Texture {
        private:
            int m_height = 0;
            int m_width = 0;
            SDL_Texture* m_texture;

            void set_dimensions(Surface& surface)
            {
                m_width = surface.get_w();
                m_height = surface.get_h();
            }
        public:
            Texture() {};

            explicit Texture(std::string path, Renderer& renderer)
            : Texture { Surface { path }, renderer }
            { }

            explicit Texture(Surface surface, Renderer& renderer)
            {
                set_dimensions(surface);
                m_texture = SDL_CreateTextureFromSurface(renderer, surface);
            }

            explicit Texture(std::string path, Renderer& renderer, int r, int g, int b)
            {
                Surface surface{ path };
                surface.set_color_key(r, g, b);

                set_dimensions(surface);

                m_texture = SDL_CreateTextureFromSurface(renderer, surface);
            }

            SDL_Texture& operator*()  { return *(m_texture); }
            operator SDL_Texture*()   { return m_texture; }

            int get_w() { return m_width; };
            int get_h() { return m_height; };

            virtual ~Texture()
            {
                if (m_texture != NULL)
                { SDL_DestroyTexture(m_texture); }
            }
    };

    class Window {
        private:
            int m_width;
            int m_height;
            SDL_Window* m_window = NULL;
            Renderer m_renderer;

        public:
            Window(int w, int h)
            : m_width { w }, m_height { h },
              m_window { SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN) },
              m_renderer { m_window, SDL_RENDERER_ACCELERATED }
            {
            };

            void clear()
            {
                set_viewport(0, 0, m_width, m_height);
                set_draw_color(0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(m_renderer);
            };

            void update()
            {
                SDL_RenderPresent(m_renderer);
            };

            Renderer& get_renderer()
            {
                return m_renderer;
            }

            Texture load_texture(std::string path)
            {
                return Texture { path, m_renderer };
            }

            void render_copy(Texture& texture)
            {
                SDL_RenderCopy(m_renderer, texture, NULL, NULL);
            }

            void set_viewport(int x, int y, int w, int h)
            {
                SDL_Rect viewport;
                viewport.x = x;
                viewport.y = y;
                viewport.w = w;
                viewport.h = h;
                SDL_RenderSetViewport(m_renderer, &viewport);
            }

            void set_draw_color(int r, int g, int b, int a)
            {
                SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
            }

            virtual ~Window()
            {
                if (m_window != NULL)
                { SDL_DestroyWindow(m_window); }
            };
    };
}
