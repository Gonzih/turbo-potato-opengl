#include <vector>

#include "random.hpp"

class Rect {
    public:
        int x0, y0, x1, y1;

        int center_x() {
            return x1 - x0 / 2;
        }

        int center_y() {
            return y1 - y0 / 2;
        }
};

class Map {
    private:
        int width;
        int height;
        std::vector<std::vector<char>> map;

        Rect gen_rect() {
            int w = width - 10;
            int h = height - 10;

            Rect rect;
            rect.x0 = rand_int(0, w);
            rect.y0 = rand_int(0, h);
            rect.x1 = rand_int(rect.x0, w);
            rect.y1 = rand_int(rect.x1, h);

            return rect;
        }

        void generate_maze() {
            int nrect = rand_int(5, 15);
            Rect rects[nrect];

            int i;

            for (i = 0; i < nrect; i++) {
                rects[i] = gen_rect();
            }

            Rect tunnels[(nrect - 1) * 2];

            for (i = 0; i < nrect - 1; i ++) {
                Rect tunnel;
            }

            // RENDER
            for (i = 0; i < nrect; i++) {
                Rect rect = rects[i];

                for (int x = rect.x0; x < rect.x1; x++) {
                    for (int y = rect.y0; y < rect.y1; y++) {
                        map[x][y] = ' ';
                    }
                }
            }
        }

    public:
        Map(int w, int h)
            : width(w), height(h), map(std::vector<std::vector<char>>(w, std::vector<char>(h, '#')))
        { generate_maze(); }

        const int get_width() { return width; }
        const int get_height() { return height; }
        const char at(int x, int y) { return map[x][y]; }
        /* void set_at(char c, int x, int y) { map[x][y] = c; } */
};
