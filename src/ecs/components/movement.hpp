#pragma once

#include <memory>
#include <functional>

#include "../ecs.hpp"
#include "../../window.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};


namespace ecs::components
{
    using CanMoveLambda = std::function<bool(Point, MovementDirection)>;

    class MovementComponent : public Component
    {
    private:
        CanMoveLambda can_move_fn;
    public:
        MovementComponent(CanMoveLambda f)
        : can_move_fn { f } {  };
        virtual ~MovementComponent() override {  };

        void move_to(Point pos)
        {
            m_entity->get_component<PositionComponent>()->set_pos(pos);
        }

        void move(MovementDirection direction)
        {
            Point pos = m_entity->get_component<PositionComponent>()->get_pos();
            switch(direction)
            {
                case MovementDirection::Up:
                    pos.y -= 1;
                    break;
                case MovementDirection::Down:
                    pos.y += 1;
                    break;
                case MovementDirection::Left:
                    pos.x -= 1;
                    break;
                case MovementDirection::Right:
                    pos.x += 1;
                    break;
                case MovementDirection::None:
                    break;
            }
            m_entity->get_component<PositionComponent>()->set_pos(pos);
        }
    };
};
