#pragma once

#include <memory>

#include "../logging.hpp"
#include "../ecs/ecs.hpp"
#include "position.hpp"
#include "map/map.hpp"

#define LIGHT_RADIUS 15

namespace ecs::components
{
    class LevelsComponent : public Component
    {
    private:
        size_t current_level = 0;
        std::vector<Map> levels;
        std::weak_ptr<Window> window;
        LightMap light_map;
        int width;
        int height;
        GetPosLambda get_pos_fn;
        SetPosLambda set_pos_fn;
    public:
        LevelsComponent(std::weak_ptr<Window> w, int width, int height, GetPosLambda get_pos_fn, SetPosLambda set_pos_fn)
        : window { w }, width { width }, height { height }, get_pos_fn { get_pos_fn }, set_pos_fn { set_pos_fn }
        { };
        virtual ~LevelsComponent() override
        {  };

        void add_map()
        {
            size_t new_level =  levels.size();
            logger::info("Initializing map level", new_level);
            Map newmap { new_level, width, height };
            levels.push_back(newmap);
        }

        void init() override
        {
            add_map();
        }

        void draw() override
        {
            char ch;
            int c;
            auto win = window.lock();

            Point pos = get_pos_fn();
            auto target_level = levels[current_level].stairs_at(pos);
            Point target_pos;
            if (target_level != -1) {
                // if target level doesn't exist (going up) add it first
                if (!(target_level < levels.size())) {
                    add_map();
                }
                target_pos = levels[target_level].stairs_to(current_level);
                logger::info("Moving to level", target_level, target_pos.x, target_pos.y);
                set_pos_fn(target_pos);
                current_level = target_level;
            }
            Map& map = levels[current_level];

            for (int i = 0; i < map.get_width(); ++i)
            {
                for (int j = 0; j < map.get_height(); ++j)
                {
                    ch = map.at(i, j);
                    c = ch;

                    if (!visible(i, j)) {
                        if (map.memoized(i, j)) {
                            c |= A_DIM;
                        } else {
                            c = '.' | A_DIM;
                        }
                    } else {
                        map.memoize(i, j);

                        if (ch == EMPTY_SPACE_CHARACTER)
                            continue;
                    }

                    win->render_char(c, i, j);
                }
            }
        }

        void regen_light_map()
        {
            auto pos = get_pos_fn();
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

        bool visible(int x, int y)
        {
            return light_map.visible(x, y);
        }

        void regen_current_map()
        {
            logger::info("Regenerating current level");
            Map newmap { current_level, width, height };
            levels[current_level] = newmap;

            auto pos = get_random_empty_coords();
            logger::info("Initializing player at (x, y)", pos.x, pos.y);
            set_pos_fn(pos);
        }
    };
};
