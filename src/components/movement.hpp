#pragma once

#include <memory>

#include "../ecs/ecs.hpp"

namespace ecs::components
{
    class MovementComponent : public Component
    {
    private:
    public:
        MovementComponent() { };
        virtual ~MovementComponent() override {  };

        void init() override {
            m_entity->assert_component<TransformComponent>("Movement");
        }

        void move(MovementDirection direction)
        {
            Vector2D pos = m_entity->get_component<TransformComponent>()->get_pos();

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

            m_entity->get_component<TransformComponent>()->set_pos(pos);
        }
    };
};
