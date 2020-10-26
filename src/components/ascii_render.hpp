#pragma once

#include <memory>

#include "../ecs/ecs.hpp"
#include "../window.hpp"
#include "levels.hpp"

namespace ecs::components
{
    class AsciiRenderComponent : public Component
    {
    private:
        std::wstring symbol;
        std::shared_ptr<Window> window;
        VisibleLambda visible_fn;
        bool respect_light_map = true;
    public:
        AsciiRenderComponent(std::wstring s, std::shared_ptr<Window> w, VisibleLambda visible_fn)
        : symbol { s }, window { w }, visible_fn { visible_fn }
        {  };
        AsciiRenderComponent(std::wstring s, std::shared_ptr<Window> w, VisibleLambda visible_fn, bool resp)
        : symbol { s }, window { w }, visible_fn { visible_fn }, respect_light_map { resp }
        {  };
        virtual ~AsciiRenderComponent() override {  };

        void set_symbol(std::wstring s)
        { symbol = s; }

        void init() override {
            m_entity->assert_component<PositionComponent>("AsciiRenderer");
        }

        void draw() override {
            auto pos = m_entity->get_component<PositionComponent>()->get_pos();
            bool visible = visible_fn(pos.x, pos.y);

            if (!respect_light_map || visible)
                window->print(symbol, pos.x, pos.y);
        }

        const std::wstring get_symbol()
        { return symbol; }
    };
};
