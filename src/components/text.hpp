#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"

namespace ecs::components
{
    class TextComponent : public Component
    {
    private:
        std::string m_text;
        GetTextLambda m_get_text;
    public:
        std::shared_ptr<sdl::Window> m_window;
        sdl::RGB m_color;
        std::shared_ptr<sdl::Texture> m_texture;

        TextComponent(std::shared_ptr<sdl::Window> w, GetTextLambda get_text, sdl::RGB color)
        : m_text { get_text() },
          m_get_text { get_text },
          m_window { w },
          m_color { color },
          m_texture { m_window->render_text(m_text, m_color) }
        {  };
        virtual ~TextComponent() override {  };

        void init() override { update(); }

        void update() override
        {
            auto txt = m_get_text();
            if (txt == m_text) return;

            m_text = txt;
            m_texture = m_window->render_text(m_text, m_color);
        }
    };
};
