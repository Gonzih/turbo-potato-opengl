#pragma once

#include <memory>

#include "../ecs/ecs.hpp"
#include "../geometry.hpp"

namespace ecs::components
{
    class OffsetComponent : public Component
    {
    private:
        Vector2D m_playfield;
        Vector2D m_map_size;
        std::shared_ptr<Entity> m_player;
    public:
        OffsetComponent(Vector2D l, Vector2D m, std::shared_ptr<Entity> p)
        : m_playfield { l }, m_map_size { m }, m_player { p }
        { };
        virtual ~OffsetComponent() override {  };

        void init() override {
            m_entity->assert_component<TransformComponent>("Offset");
        }

        bool in_fov(Vector2D pos)
        {
            auto offset = m_entity->get_component<TransformComponent>()->get_pos();
            auto new_pos = pos + offset;
            return new_pos.x >= 0 && new_pos.y >= 0 && new_pos.x < m_playfield.x && new_pos.y < m_playfield.y;
        }

        bool in_fov(int x, int y)
        {
            return in_fov(Vector2D { x, y });
        }

        void reset()
        {
            auto offset = m_entity->get_component<TransformComponent>()->get_pos();
            auto player = m_player->get_component<TransformComponent>()->get_pos();

            offset.x = m_playfield.x/2 - player.x;
            offset.y = m_playfield.y/2 - player.y;

            m_entity->get_component<TransformComponent>()->set_pos(offset);
        }

        void update() override
        {
            reset();

            auto offset = m_entity->get_component<TransformComponent>()->get_pos();
            auto player = m_player->get_component<TransformComponent>()->get_pos();

            if (offset.x > 0) {
                offset.x = 0;
            }

            if (offset.y > 0) {
                offset.y = 0;
            }

            if (player.x + m_playfield.x/2 > m_map_size.x) {
                offset.x = -(m_map_size.x - m_playfield.x);
            }

            if (player.y + m_playfield.y/2 > m_map_size.y) {
                offset.y = -(m_map_size.y - m_playfield.y);
            }

            m_entity->get_component<TransformComponent>()->set_pos(offset);
        }
    };
};
