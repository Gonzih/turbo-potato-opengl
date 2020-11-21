#pragma once

#include <memory>

#include "../ecs/ecs.hpp"
#include "../geometry.hpp"

namespace ecs::components
{
    class OffsetComponent : public Component
    {
    private:
        Vector2D m_limit;
    public:
        OffsetComponent(Vector2D limit) : m_limit { limit } { };
        virtual ~OffsetComponent() override {  };

        void init() override {
            m_entity->assert_component<TransformComponent>("Offset");
        }

        void update() override
        {
            auto pos = m_entity->get_component<TransformComponent>()->get_pos();

            // get player coords here
            // DO SOME MATH OR SOMETHING

            if (pos.x > 0) pos.x = 0;
            if (pos.y > 0) pos.y = 0;
            m_entity->get_component<TransformComponent>()->set_pos(pos);

            if (pos.x > m_limit.x) pos.x = m_limit.x;
            if (pos.y > m_limit.y) pos.y = m_limit.y;
        }
    };
};
