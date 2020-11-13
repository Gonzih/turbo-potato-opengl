#pragma once

#include <memory>

#include "../ecs/ecs.hpp"
#include "../geometry.hpp"

namespace ecs::components
{
    class PositionComponent : public Component
    {
    private:
        Vector2D pos;
    public:
        PositionComponent(Vector2D p) : pos { p } {  };
        PositionComponent(int x, int y) : pos { x, y } {  };
        virtual ~PositionComponent() override {  };

        void set_pos(Vector2D np) { pos = np; };
        const Vector2D get_pos() { return pos; };
        const int get_x() { return pos.x; };
        const int get_y() { return pos.y; };
    };
};
