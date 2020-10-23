#pragma once

#include <memory>

#include "../ecs.hpp"
#include "../../window.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs::components
{
    class MovementComponent : public Component
    {
    public:
        MovementComponent() {  };
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
