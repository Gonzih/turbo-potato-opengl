#pragma once

#include <memory>

#include "../ecs.hpp"
#include "map/map.hpp"

#define LIGHT_RADIUS 15

namespace ecs::components
{
    class LevelsComponent : public Component
    {
    private:
        size_t current_level = 0;
        std::vector<Map> levels;
        std::weak_ptr<Entity> player;
        std::weak_ptr<Window> window;
        LightMap light_map;
        int width;
        int height;
    public:
        LevelsComponent(std::weak_ptr<Entity> p, std::weak_ptr<Window> w, int width, int height)
        : player { p }, window { w }, width { width }, height { height }
        { };
        virtual ~LevelsComponent() override
        {  };

        void init() override
        {
            logger::info("Initializing first map");
            Map newmap { width, height };
            levels.push_back(newmap);
        }

        void draw() override
        {
            int c;
            Map& map = levels[current_level];
            auto win = window.lock();

            for (int i = 0; i < map.get_width(); ++i)
            {
                for (int j = 0; j < map.get_height(); ++j)
                {
                    c = map.at(i, j);

                    if (!light_map.visible(i, j)) {
                        if (map.memoized(i, j)) {
                            c |= A_DIM;
                        } else {
                            c = '.' | A_DIM;
                        }
                    } else {
                        map.memoize(i, j);
                    }

                    logger::info("Rendering char");
                    win->render_char(c, i, j);
                }
            }
        }

        void regen_light_map()
        {
            logger::info("Regenerating light map");
            auto pos = player.lock()->get_component<PositionComponent>()->get_pos();
            light_map = levels[current_level].generate_light_map(pos, LIGHT_RADIUS);
        }

        void update() override
        { regen_light_map(); }

        bool can_move(Point pos, MovementDirection direction) const
        {
            return levels[current_level].can_move(pos, direction);
        }

        Point get_random_empty_coords() const
        {
            return levels[current_level].get_random_empty_coords();
        }

        void regen_current_map()
        {
            logger::info("Regenerating current level");
            Map newmap { width, height };
            levels[current_level] = newmap;

            auto pos = get_random_empty_coords();
            logger::info("Initializing player at (x, y)", pos.x, pos.y);
            player->get_component<PositionComponent>()->move_to(pos);
        }
    };
};
