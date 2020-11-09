#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "position.hpp"
#include "sprite.hpp"

namespace ecs::components
{
    class SpriteRenderComponent : public Component
    {
    private:
    public:
        SpriteRenderComponent() {  };
        virtual ~SpriteRenderComponent() override {  };

        void init() override {
            m_entity->assert_component<SpriteComponent>("SpriteRenderer");
        }

        void draw() override {
            auto pos = m_entity->get_component<PositionComponent>()->get_pos();

            static int frame = 0;
            static SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
            /* static int sprite_width = 64; */
            /* static int sprite_height = 205; */
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;
            auto window = m_entity->get_component<SpriteComponent>()->m_window;

            ++frame;
            int f = frame / 5;
            if (f == 4) {
                frame = 0;
                f = 0;
            }
            sprite->render(window->get_renderer(), f, 0, pos.x, pos.y, 0, NULL, flip);
        }
    };
};
