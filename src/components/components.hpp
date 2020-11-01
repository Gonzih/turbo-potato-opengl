#pragma once

#include <functional>

#include "../geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs::components {
    class PositionComponent;
    class SpriteRenderComponent;
    // class LevelsComponent;
    // class MovementComponent;

    using SetPosLambda = std::function<void(Point)>;
    using GetPosLambda = std::function<Point()>;
    using CanMoveLambda = std::function<bool(Point, MovementDirection)>;
    using VisibleLambda = std::function<bool(int, int)>;
};

#include "position.hpp"
#include "sprite_render.hpp"
// #include "levels.hpp"
// #include "movement.hpp"
