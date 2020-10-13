#include <algorithm>
#include <vector>
#include <utility>
#include <math.h>

#include "random.hpp"
#include "logging.hpp"

#define WALL_CHARACTER '#'
#define EMPTY_SPACE_CHARACTER ' '

enum LightLevel {
    Invisible,
    Dim,
    Visible
};

class LightMap {
    private:
        std::vector<std::vector<LightLevel> > light_map;
        double distance(std::pair<int, int> a, std::pair<int, int> b) {
            return sqrt(pow(double(a.first - b.first), 2.0) + pow(double(a.second - b.second), 2.0));
        };

        bool los(std::pair<int, int> a, std::pair<int, int> b, std::vector<std::vector<char> > &map) {
            auto [ax, ay] = a;
            auto [bx, by] = b;
            /* std::pair<int, int> target; */

            double vx, vy, ox, oy, l;
            int i;
            vx = ax - bx;
            vy = ay - by;
            ox = (double)ax * 0.5;
            oy = (double)ay * 0.5;
            l = sqrt(vx*vx+vy*vy);
            vx /= l;
            vy /= l;

            for (i=0;i<(int)l;i++) {
                if (map[(int)ox][(int)oy] == WALL_CHARACTER) {
                    /* return false; */
                }

                ox += vx;
                oy += vy;
            }

            return true;
        }
    public:
        LightMap(std::pair<int, int> camera_pos, int w, int h, std::vector<std::vector<char> > &map, double light_radius)
        : light_map(std::vector<std::vector<LightLevel> >(w, std::vector<LightLevel>(h, LightLevel::Dim)))
        {
            auto [cx, cy] = camera_pos;
            int ilr = int(light_radius + 1.0);
            std::pair<int, int> target;

            for (int x = std::max(0, cx - ilr); x < std::min(w, cx + ilr); x++) {
                for (int y = std::max(0, cy - ilr); y < std::min(h, cy + ilr); y++) {
                    target = std::make_pair(x, y);

                    if (los(target, camera_pos, map) && distance(target, camera_pos) < light_radius) {
                        light_map[x][y] = LightLevel::Visible;
                    }
                }
            }
        };

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

        void add_tunnel_to_existing(std::vector<std::vector<char>> &map,
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

        void render(std::vector<std::vector<char>> &map) {
            // renders the rectangle on the map
            for (int x = x0; x < x1; x++) {
                for (int y = y0; y < y1; y++) {
                    map[x][y] = EMPTY_SPACE_CHARACTER;
                }
            }
        }
};

class Map {
    private:
        int width;
        int height;
        std::vector<std::vector<char>> map;
        int nrect = 2;
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
            logger->info("Maze number of rectangles={}", nrect);
            std::vector<Rect> rects;

            for (int i = 0; i < nrect; i++) {
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
            map(std::vector<std::vector<char>>(w, std::vector<char>(h, WALL_CHARACTER)))
        { generate_maze(); }

        const int get_width() { return width; }
        const int get_height() { return height; }
        const char at(int x, int y) { return map[x][y]; }

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

        LightMap generate_light_map(std::pair<int, int> camera_pos, double light_radius) {
            return LightMap(camera_pos, width, height, map, light_radius);
        };
};
