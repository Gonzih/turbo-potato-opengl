#include <algorithm>
#include <vector>
#include <utility>
#include <math.h>

#include "random.hpp"
#include "logging.hpp"
#include "character.hpp"

#define WALL_CHARACTER '#'
#define EMPTY_SPACE_CHARACTER ' '

class Tile {
    public:
        Tile(char c_): c(c_) { };
        char c = WALL_CHARACTER;
        bool memoized = false;
    private:
};
const Tile wall_tile {WALL_CHARACTER};

enum LightLevel {
    Invisible,
    Dim,
    Visible
};

class LightMap {
    private:
        std::vector<std::vector<LightLevel>> light_map;

        // Implementation based on this pseudo code http://www.roguebasin.com/index.php?title=Eligloscode
        void calc_fov(float x, float y, int w, int h, std::pair<int, int> camera_pos, const std::vector<std::vector<Tile>> &map, int light_radius) {
            auto [camera_x, camera_y] = camera_pos;

            int i, tx, ty;
            float ox, oy;
            ox = static_cast<float>(camera_x) + 0.5f;
            oy = static_cast<float>(camera_y) + 0.5f;

            for (i = 0; i < light_radius; ++i) {
                tx = static_cast<int>(ox);
                ty = static_cast<int>(oy);

                if (!(tx >= 0 && tx <= w && ty >= 0 && ty <= h)) {
                    return;
                }

                light_map[tx][ty] = LightLevel::Visible;

                if (map[tx][ty].c == WALL_CHARACTER) {
                    return;
                }

                ox += x;
                oy += y;
            }
        }
    public:
        LightMap(std::pair<int, int> camera_pos, int w, int h, const std::vector<std::vector<Tile>> &map, float light_radius)
        : light_map(std::vector<std::vector<LightLevel>>(w, std::vector<LightLevel>(h, LightLevel::Dim)))
        {
            float x, y, fi;

            for (int i = 0; i < 360; ++i) {
                fi = static_cast<float>(i);
                x = cos(fi*0.01745f);
                y = sin(fi*0.01745f);

                calc_fov(x, y, w, h, camera_pos, map, light_radius);
            }
        };

        bool visible(int x, int y) {
            return light_level(x, y) == LightLevel::Visible;
        }

        LightLevel light_level(int x, int y) {
            return light_map[x][y];
        };
};

class Rect {
    public:
        int x0, y0, x1, y1;

        int center_x() {
            return ((x1 - x0) / 2) + x0;
        }

        int center_y() {
            return ((y1 - y0) / 2) + y0;
        }

        void add_tunnel_to_existing(std::vector<std::vector<Tile>> &map,
                std::vector<Rect> &existing_rects) {
            if (existing_rects.size() > 0) {
                // Decide which rectangle to connect to
                // Rect rect = existing_rects[rand_int(0, existing_rects.size()) - 1]; // FIXATSOMEPOINT: segfaults
                Rect rect = existing_rects.back();

                // Calculate centers and sort
                int centers_x[2] = { center_x(), rect.center_x() };
                int centers_y[2] = { center_y(), rect.center_y() };
                // hacky fix this is needed to handle different types of elbows -| and |-
                int tunnel_y_x_index = ((centers_x[0] > centers_x[1]) && (centers_y[0] > centers_y[1])) ? 1 : 0;
                if (centers_x[0] > centers_x[1])
                    std::swap(centers_x[0], centers_x[1]);
                if (centers_y[0] > centers_y[1])
                    std::swap(centers_y[0], centers_y[1]);

                // Make the horizontal part of the tunnel
                Rect tunnel_x;
                tunnel_x.x0 = centers_x[0];
                tunnel_x.y0 = centers_y[0]-1;
                tunnel_x.x1 = centers_x[1];
                tunnel_x.y1 = centers_y[0]+1;
                tunnel_x.render(map);
                // Make the vertical part of the tunnel
                Rect tunnel_y;
                tunnel_y.x0 = centers_x[tunnel_y_x_index] - 1;
                tunnel_y.y0 = centers_y[0];
                tunnel_y.x1 = centers_x[tunnel_y_x_index] + 1;
                tunnel_y.y1 = centers_y[1];
                tunnel_y.render(map);
            }
        }

        void render(std::vector<std::vector<Tile>> &map) {
            // renders the rectangle on the map
            for (int x = x0; x < x1; ++x) {
                for (int y = y0; y < y1; ++y) {
                    map[x][y].c = EMPTY_SPACE_CHARACTER;
                }
            }
        }
};

class Map {
    private:
        int width;
        int height;
        std::vector<std::vector<Tile>> map;
        int nrect = rand_int(8, 16);
        std::vector<Rect> rects;

        Rect gen_rect(int size_w_limit, int size_h_limit) {
            int size_w = rand_int(3, size_w_limit);
            int size_h = rand_int(3, size_h_limit);
            Rect rect;

            // start of rect can be anywhere on screen
            rect.x0 = rand_int(0, width-size_w);
            rect.y0 = rand_int(0, height-size_h);
            // add size to starting point
            rect.x1 = rect.x0 + size_w;
            rect.y1 = rect.y0 + size_h;

            return rect;
        }

        void generate_maze() {
            log::info("Maze number of rectangles is", nrect);

            for (int i = 0; i < nrect; ++i) {
                auto rect = gen_rect(width/4, height/4);
                // renders the rectangle on the map
                rect.render(map);
                // connects rectangle to existing rectangles
                rect.add_tunnel_to_existing(map, rects);
                rects.push_back(rect);
            }
        }

    public:
        Map(int w, int h) :
            width(w),
            height(h),
            map(std::vector<std::vector<Tile>>(w, std::vector<Tile>(h, wall_tile)))
        {
            generate_maze();
        }

        const int get_width() { return width; }
        const int get_height() { return height; }
        const char at(int x, int y) { return map[x][y].c; }
        const bool memoized(int x, int y) { return map[x][y].memoized; }
        void memoize(int x, int y) { map[x][y].memoized = true; }

        std::pair<int, int> get_random_empty_coords() {
            int x, y;
            for (;;) {
                x = rand_int(0, width);
                y = rand_int(0, height);
                if (at(x, y) == EMPTY_SPACE_CHARACTER) {
                    return std::make_pair(x, y);
                }
            }
        }

        bool can_move(std::pair<int, int> pos, MovementDirection direction) {
            switch(direction) {
                case MovementDirection::Up:
                    pos.second -= 1;
                    break;
                case MovementDirection::Down:
                    pos.second += 1;
                    break;
                case MovementDirection::Left:
                    pos.first -= 1;
                    break;
                case MovementDirection::Right:
                    pos.first += 1;
                    break;
                case MovementDirection::None:
                    break;
            };

            auto [x, y] = pos;

            return x >= 0 && y >= 0 && x <= width && y <= height && map[x][y].c != WALL_CHARACTER;
        };

        LightMap generate_light_map(std::pair<int, int> camera_pos, int light_radius) {
            return LightMap(camera_pos, width, height, map, light_radius);
        };
};
