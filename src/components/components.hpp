#pragma once

#include <functional>

#include "../geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs::components {
    class TransformComponent;
    class SpriteRenderComponent;
    class MovementComponent;

    using SetPosLambda = std::function<void(Vector2D)>;
    using GetPosLambda = std::function<Vector2D()>;
    using CanMoveLambda = std::function<bool(Vector2D, MovementDirection)>;
    using VisibleLambda = std::function<bool(int, int)>;
    using MemoizedLambda = std::function<bool(int, int)>;
};

#include "transform.hpp"
#include "sprite.hpp"
#include "sprite_render.hpp"
#include "movement.hpp"
#include "darkness.hpp"
