#pragma once

#include <memory>

#include "../ecs.hpp"
#include "../../window.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

/* namespace ecs::components */
/* { */
/*     class MovementComponent : public Component */
/*     { */
/*     private: */
/*         std::weak_ptr<Map> map; */
/*     public: */
/*         MovementComponent(std::shared_ptr<Window> w) */
/*         : symbol { s }, window { w } */
/*         {  }; */
/*         virtual ~MovementComponent() override {  }; */

/*         void set_symbol(std::wstring s) */
/*         { symbol = s; } */

/*         void draw() override { */
/*             window->print(symbol, */
/*                     m_entity->get_component<PositionComponent>()->get_x(), */
/*                     m_entity->get_component<PositionComponent>()->get_y()); */
/*         } */

/*         const std::wstring get_symbol() */
/*         { return symbol; } */
/*     }; */
/* }; */
