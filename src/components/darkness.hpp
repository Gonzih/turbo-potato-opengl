#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "components.hpp"

namespace ecs::components
{
    class DarknessComponent : public Component
    {
    private:
        int m_width;
        int m_height;
        VisibleLambda m_is_visible;
        MemoizedLambda m_is_memoized;
        std::shared_ptr<Entity> m_offset;

    public:
        DarknessComponent(int width, int height, VisibleLambda vfn, MemoizedLambda mfn, std::shared_ptr<Entity> offset)
        : m_width { width }, m_height { height }, m_is_visible { vfn }, m_is_memoized { mfn }, m_offset { offset }
        {  }
        virtual ~DarknessComponent() override {  }

        void init() override
        {
            m_entity->assert_component<TransformComponent>("DarknessComponent");
            m_entity->assert_component<SpriteComponent>("DarknessComponent");
        }
        void draw() override
        {

            auto window = m_entity->get_component<SpriteComponent>()->m_window;
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;
            auto sprite_w = sprite->get_w();
            auto sprite_h = sprite->get_h();

            Vector2D offset_pos;
            auto offset = m_offset->get_component<OffsetComponent>();
            auto [ofx, ofy] = m_offset->get_component<TransformComponent>()->get_pos();

            int xx, yy;
            for (int x = 0; x < m_width; ++x)
            {
                for (int y = 0; y < m_height; ++y)
                {
                    if (offset->in_fov(x, y) && !m_is_visible(x, y)) {
                        if (m_is_memoized(x, y)) {
                            sprite->set_alpha(150);
                        } else {
                            sprite->set_alpha(255);
                        }

                        xx = x + ofx;
                        yy = y + ofy;

                        sprite->render(window->get_renderer(), 0, 0, xx*sprite_w, yy*sprite_h, 0, NULL);
                    }
                }
            }
        }
    };
};
