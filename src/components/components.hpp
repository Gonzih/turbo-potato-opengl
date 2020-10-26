#pragma once

#include <functional>

#include "../geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs::components {
    class PositionComponent;
    class AsciiRenderComponent;
    class LevelsComponent;
    class MovementComponent;

    using SetPosLambda = std::function<void(Point)>;
    using GetPosLambda = std::function<Point()>;
    using CanMoveLambda = std::function<bool(Point, MovementDirection)>;
    using VisibleLambda = std::function<bool(int, int)>;
};

#include "components/position.hpp"
#include "components/ascii_render.hpp"
#include "components/levels.hpp"
#include "components/movement.hpp"
