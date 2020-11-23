#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "transform.hpp"
#include "sprite.hpp"
#include "offset.hpp"

namespace ecs::components
{
    class TextRenderComponent : public Component
    {
    private:
    public:
        TextRenderComponent() {  };

        virtual ~TextRenderComponent() override {  };

        void init() override {
            m_entity->assert_component<TransformComponent>("TextRenderer");
        }

        void draw() override {
            auto [x, y] = m_entity->get_component<TransformComponent>()->get_pos();

            auto window = m_entity->get_component<TextComponent>()->m_window;
            auto texture = m_entity->get_component<TextComponent>()->m_texture;

            texture->render(window->get_renderer(), x, y);
        }
    };
};
