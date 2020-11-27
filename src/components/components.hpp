#pragma once

#include <functional>

#include "../geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

inline MovementDirection opposite_direction(MovementDirection direction)
{
    switch(direction)
    {
        case MovementDirection::Up:
            return MovementDirection::Down;
            break;
        case MovementDirection::Down:
            return MovementDirection::Up;
            break;
        case MovementDirection::Left:
            return MovementDirection::Right;
            break;
        case MovementDirection::Right:
            return MovementDirection::Left;
            break;
        case MovementDirection::None:
            return MovementDirection::None;
            break;
    }
};

namespace ecs::components {
    class TransformComponent;
    class SpriteRenderComponent;
    class MovementComponent;
};

#include "transform.hpp"
#include "movement.hpp"
