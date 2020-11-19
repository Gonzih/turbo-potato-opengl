#pragma once

#include <memory>

#include "../ecs/ecs.hpp"
#include "../geometry.hpp"

namespace ecs::components
{
    class TransformComponent : public Component
    {
    private:
        Vector2D pos;
    public:
        TransformComponent(Vector2D p) : pos { p } {  };
        TransformComponent(int x, int y) : pos { x, y } {  };
        virtual ~TransformComponent() override {  };

        void set_pos(Vector2D np) { pos = np; };
        const Vector2D get_pos() { return pos; };
        const int get_x() { return pos.x; };
        const int get_y() { return pos.y; };
    };
};
