#include "levels.hpp"

namespace ecs::components
{
        LevelsComponent::LevelsComponent(int width, int height, int sprite_size, GetPosLambda get_pos_fn, SetPosLambda set_pos_fn)
        : levels {  }, width { width }, height { height }, sprite_size { sprite_size }, get_pos_fn { get_pos_fn }, set_pos_fn { set_pos_fn }
        { };
        LevelsComponent::~LevelsComponent()
        {  };

        void LevelsComponent::add_map()
        {
            auto id =  levels.size();
            /* logger::info("Initializing map level"); */
            Map newmap { id, width, height };
            levels.push_back(newmap);
            current_level = id;
        }

        void LevelsComponent::init()
        {
            add_map();
        }

        void LevelsComponent::draw()
        {
            char ch;
            int c;
            auto window = m_entity->get_component<SpriteComponent>()->m_window;
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;

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
                        } else {
                            c = '.';
                        }
                    } else {
                        map.memoize(i, j);

                        if (ch == EMPTY_SPACE_CHARACTER)
                            continue;
                    }

                    if (c == '#') {
                        logger::info("Rendering char", "at", i, j);
                        sprite->render(window->get_renderer(), 0, 0, i*sprite_size, j*sprite_size, 0, NULL);
                    }
                }
            }
        }

        void LevelsComponent::regen_light_map()
        {
            auto pos = get_pos_fn();
            light_map = levels[current_level].generate_light_map(pos, LIGHT_RADIUS);
        }

        void LevelsComponent::update()
        { regen_light_map(); }

        bool LevelsComponent::can_move(Point pos, MovementDirection direction) const
        {
            return levels[current_level].can_move(pos, direction);
        }

        Point LevelsComponent::get_random_empty_coords() const
        {
            return levels[current_level].get_random_empty_coords();
        }

        bool LevelsComponent::visible(int x, int y)
        {
            return light_map.visible(x, y);
        }

        void LevelsComponent::regen_current_map()
        {
            logger::info("Regenerating current level");
            Map newmap { current_level, width, height };
            levels[current_level] = newmap;

            auto pos = get_random_empty_coords();
            logger::info("Initializing player at (x, y)", pos.x, pos.y);
            set_pos_fn(pos);
        }
};
