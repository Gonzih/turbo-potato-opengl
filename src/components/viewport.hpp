#pragma once

#include <memory>

#include "../sdl/sdl.hpp"
#include "../ecs/ecs.hpp"
#include "../geometry.hpp"

namespace ecs::components
{
    class ViewportComponent : public Component
    {
    private:
        std::shared_ptr<sdl::Window> m_window;
        Vector2D m_start;
        Vector2D m_size;
        Vector2D m_sprite_size;
        Vector2D m_source_size;

        const Vector2D top_left_corner(Vector2D center)
        {
            int sx;
            sx = std::max(0, center.x*m_sprite_size.x - m_size.x/2);
            sx = std::min(sx, m_source_size.x - m_size.x);

            int sy;
            sy = std::max(0, center.y*m_sprite_size.y - m_size.y/2);
            sy = std::min(sy, m_source_size.y - m_size.y);

            return Vector2D { sx, sy };
        }
    public:
        ViewportComponent(std::shared_ptr<sdl::Window> w, Vector2D start, Vector2D size, Vector2D sprite_size, Vector2D source_size)
        : m_window { w }, m_start { start }, m_size { size }, m_sprite_size { sprite_size }, m_source_size { source_size }
        {  }

        virtual ~ViewportComponent() override {  }

        const int get_x() { return m_start.x; }
        const int get_y() { return m_start.y; }
        const int get_w() { return m_size.x; }
        const int get_h() { return m_size.y; }

        const void set_viewport() {
            m_window->set_viewport(m_start.x, m_start.y, m_size.x, m_size.y);
        }

        const void reset_viewport() {
            m_window->reset_viewport();
        }

        const bool visible(int x, int y, Vector2D center)
        {
            auto remap = map_coords(x, y, center);
            return remap.x > 0 && remap.y > 0;
        }

        const Vector2D map_coords(int x, int y, Vector2D center)
        {
            auto tlc = top_left_corner(center);
            return Vector2D { (x*m_sprite_size.x - tlc.x), (y*m_sprite_size.y - tlc.y) };
        }
    };
};
