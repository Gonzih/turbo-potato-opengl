#pragma once

#include <functional>

#include "../geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs::components {
    class PositionComponent;
    class SpriteRenderComponent;
    class LevelsComponent;
    class MovementComponent;

    using SetPosLambda = std::function<void(Vector2D)>;
    using GetPosLambda = std::function<Vector2D()>;
    using CanMoveLambda = std::function<bool(Vector2D, MovementDirection)>;
    using VisibleLambda = std::function<bool(int, int)>;
    using MemoizedLambda = std::function<bool(int, int)>;
};

#include "position.hpp"
#include "sprite.hpp"
#include "sprite_render.hpp"
#include "levels.hpp"
#include "movement.hpp"
#include "darkness.hpp"
