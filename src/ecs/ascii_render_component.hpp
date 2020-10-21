#pragma once

#include <memory>

#include "ecs.hpp"
#include "../window.hpp"

namespace ecs::components
{
    class AsciiRenderComponent : public Component
    {
    private:
        std::wstring symbol;
        std::shared_ptr<Window> window;
    public:
        AsciiRenderComponent(std::wstring s, std::shared_ptr<Window> w)
        : symbol { s }, window { w }
        {  };
        virtual ~AsciiRenderComponent() override {  };

        void set_symbol(std::wstring s)
        { symbol = s; }

        void draw() override {
            window->print(symbol,
                    entity->get_component<PositionComponent>()->get_x(),
                    entity->get_component<PositionComponent>()->get_y());
        }

        const std::wstring get_symbol()
        { return symbol; }
    };
};
