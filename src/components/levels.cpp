#include "levels.hpp"

namespace ecs::components
{
        LevelsComponent::LevelsComponent(int width, int height, GetPosLambda get_pos_fn, SetPosLambda set_pos_fn)
        : difficulty { 0 }, width { width }, height { height }, get_pos_fn { get_pos_fn }, set_pos_fn { set_pos_fn }
        { };
        LevelsComponent::~LevelsComponent()
        {  };

        void LevelsComponent::add_map()
        {
            difficulty++;
            logger::info("Initializing map level");
            level = std::make_unique<Map>(width, height);
        }

        void LevelsComponent::init()
        {
            m_entity->assert_component<SpriteComponent>("LevelsComponent");
            add_map();
        }

        void LevelsComponent::draw()
        {
            TileType tile;
            auto window = m_entity->get_component<SpriteComponent>()->m_window;
            auto sprite = m_entity->get_component<SpriteComponent>()->m_sprite;
            auto sprite_w = sprite->get_width();
            auto sprite_h = sprite->get_height();

            int sprite_col = 0;
            for (int i = 0; i < level->get_width(); ++i)
            {
                for (int j = 0; j < level->get_height(); ++j)
                {
                    tile = level->at(i, j);

                    if (visible(i, j)) {
                        level->memoize(i, j);
                    }

                    switch (tile)
                    {
                        case TileType::Wall:
                            sprite_col = 0;
                            break;
                        case TileType::Empty:
                            sprite_col = 1;
                            break;
                        case TileType::StairsDown:
                            sprite_col = 2;
                            break;
                        case TileType::StairsUp:
                            break;
                    }

                    sprite->render(window->get_renderer(), 0, sprite_col, i*sprite_w, j*sprite_h, 0, NULL);
                }
            }
        }

        void LevelsComponent::regen_light_map()
        {
            auto pos = get_pos_fn();
            light_map = level->generate_light_map(pos, LIGHT_RADIUS);
        }

        void LevelsComponent::update()
        { regen_light_map(); }

        bool LevelsComponent::can_move(Point pos, MovementDirection direction) const
        {
            return level->can_move(pos, direction);
        }

        bool LevelsComponent::can_go_downstairs(Point pos) const
        {
            return level->at(pos.x, pos.y) == TileType::StairsDown;
        }

        Point LevelsComponent::get_random_empty_coords() const
        {
            return level->get_random_empty_coords();
        }

        bool LevelsComponent::visible(int x, int y)
        {
            return light_map.visible(x, y);
        }

        bool LevelsComponent::memoized(int x, int y)
        {
            return level->memoized(x, y);
        }

        void LevelsComponent::go_down_level()
        {
            add_map();

            auto pos = get_random_empty_coords();
            logger::info("Initializing player at (x, y)", pos.x, pos.y);
            set_pos_fn(pos);
        }

        int LevelsComponent::get_difficulty()
        { return difficulty; }
};
