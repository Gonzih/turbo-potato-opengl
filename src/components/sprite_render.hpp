#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "transform.hpp"
#include "sprite.hpp"

namespace ecs::components
{
    class SpriteRenderComponent : public Component
    {
    private:
        int m_col;
        int m_row;
        VisibleLambda m_is_visible;
    public:
        SpriteRenderComponent(VisibleLambda vfn)
        : m_col { 0 }, m_row { 0 }, m_is_visible { vfn } {  };
        SpriteRenderComponent(int col, int row, VisibleLambda vfn)
        : m_col { col }, m_row { row }, m_is_visible { vfn } {  };

        virtual ~SpriteRenderComponent() override {  };

        void init() override {
            m_entity->assert_component<SpriteComponent>("SpriteRenderer");
        }

        void draw() override {
            auto pos = m_entity->get_component<TransformComponent>()->get_pos();

            static SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;
            auto window = m_entity->get_component<SpriteComponent>()->m_window;
            auto w = sprite->get_w();
            auto h = sprite->get_h();

            if (m_is_visible(pos.x, pos.y))
                sprite->render(window->get_renderer(), m_col, m_row, pos.x*w, pos.y*h, 0, NULL, flip);
        }
    };
};
