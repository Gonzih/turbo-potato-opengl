#pragma once

#include <memory>

#include "../ecs.hpp"
#include "../../window.hpp"
#include "levels.hpp"

namespace ecs::components
{
    class AsciiRenderComponent : public Component
    {
    private:
        std::wstring symbol;
        std::shared_ptr<Window> window;
        std::weak_ptr<Entity> levels;
        bool respect_light_map = true;
    public:
        AsciiRenderComponent(std::wstring s, std::shared_ptr<Window> w, std::weak_ptr<Entity> l)
        : symbol { s }, window { w }, levels { l }
        {  };
        AsciiRenderComponent(std::wstring s, std::shared_ptr<Window> w, std::weak_ptr<Entity> l, bool resp)
        : symbol { s }, window { w }, levels { l }, respect_light_map { resp }
        {  };
        virtual ~AsciiRenderComponent() override {  };

        void set_symbol(std::wstring s)
        { symbol = s; }

        void init() override {
            if (!m_entity->has_component<PositionComponent>())
                throw std::runtime_error("AscriiRenderComponent requires entity to have PositionComponent also");
        }

        void draw() override {
            auto pos = m_entity->get_component<PositionComponent>()->get_pos();
            bool visible = levels.lock()->get_component<LevelsComponent>()->visible(pos.x, pos.y);

            if (!respect_light_map || visible)
                window->print(symbol, pos.x, pos.y);
        }

        const std::wstring get_symbol()
        { return symbol; }
    };
};
