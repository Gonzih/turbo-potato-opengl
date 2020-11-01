#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "position.hpp"

namespace ecs::components
{
    class SpriteRenderComponent : public Component
    {
    private:
        std::shared_ptr<sdl::Window> window;
    public:
        SpriteRenderComponent(std::shared_ptr<sdl::Window> w)
        : window { w }
        {  };
        virtual ~SpriteRenderComponent() override {  };

        void init() override {
            m_entity->assert_component<PositionComponent>("SpriteRenderer");
        }

        void draw() override {
            auto pos = m_entity->get_component<PositionComponent>()->get_pos();

            static int frame = 0;
            static SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
            static int sprite_width = 64;
            static int sprite_height = 205;
            auto sprite = window->load_sprite("foo.png", 1, 4, sprite_width, sprite_height, sdl::RGB { 0, 0xFF, 0xFF });

            ++frame;
            int f = frame / 5;
            if (f == 4) {
                frame = 0;
                f = 0;
            }
            sprite.render(window->get_renderer(), f, 0, pos.x, pos.y, 0, NULL, flip);
        }
    };
};
