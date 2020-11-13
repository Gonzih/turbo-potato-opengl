#pragma once

#include <memory>

#include "../logging.hpp"
#include "../ecs/ecs.hpp"
#include "components.hpp"
#include "position.hpp"
#include "map/map.hpp"

#define LIGHT_RADIUS 15

namespace ecs::components
{
    class LevelsComponent : public Component
    {
    private:
        int difficulty = 0;
        std::unique_ptr<Map> level;
        LightMap light_map;
        int width;
        int height;
        GetPosLambda get_pos_fn;
        SetPosLambda set_pos_fn;
    public:
        LevelsComponent(int width, int height, GetPosLambda get_pos_fn, SetPosLambda set_pos_fn);
        virtual ~LevelsComponent() override;

        void init() override;
        void draw() override;
        void update() override;

        void add_map();
        void regen_light_map();

        bool can_move(Vector2D pos, MovementDirection direction) const;
        bool can_go_downstairs(Vector2D pos) const;
        Vector2D get_random_empty_coords() const;
        bool visible(int x, int y);
        bool memoized(int x, int y);
        void go_down_level();

        int get_difficulty();
    };
};
