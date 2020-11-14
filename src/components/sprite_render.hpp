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
        VisibleLambda m_is_visible;
    public:
        SpriteRenderComponent(VisibleLambda vfn)
        : m_is_visible { vfn } {  };
        virtual ~SpriteRenderComponent() override {  };

        void init() override {
            m_entity->assert_component<SpriteComponent>("SpriteRenderer");
        }

        void draw() override {
            auto pos = m_entity->get_component<PositionComponent>()->get_pos();

            static SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;
            auto window = m_entity->get_component<SpriteComponent>()->m_window;
            auto w = sprite->get_w();
            auto h = sprite->get_h();

            if (m_is_visible(pos.x, pos.y))
                sprite->render(window->get_renderer(), 0, 0, pos.x*w, pos.y*h, 0, NULL, flip);
        }
    };
};
