#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"

namespace ecs::components
{
    class SpriteComponent : public Component
    {
    public:
        std::shared_ptr<sdl::Window> m_window;
        std::shared_ptr<sdl::Sprite> m_sprite;

        SpriteComponent(std::shared_ptr<sdl::Window> w, std::shared_ptr<sdl::Sprite> sprite)
        : m_window { w }, m_sprite { sprite }
        {  };
        virtual ~SpriteComponent() override {  };

        void init() override { }
        void draw() override { }
    };
};
