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

        void render(std::vector<std::vector<char> > * map) {
            for (int x = x0; x < x1; x++) {
                for (int y = y0; y < y1; y++) {
                    (*map)[x][y] = ' ';
                }
            }
        }
};

class Map {
    private:
        int width;
        int height;
        std::vector<std::vector<char> > map;

        Rect gen_rect(int size_w, int size_h) {
            Rect rect;
            // start of rect can be anywhere on screen
            rect.x0 = rand_int(1, width-size_w-1);
            rect.y0 = rand_int(1, height-size_h-1);
            // add size to starting point
            rect.x1 = rand_int(rect.x0, rect.x0 + size_w);
            rect.y1 = rand_int(rect.x1, rect.y0 + size_h);

            return rect;
        }

        void generate_maze() {
            int nrect = rand_int(5, 15);
            Rect rects[nrect];

            int i;

            for (i = 0; i < nrect; i++) {
                rects[i] = gen_rect(width/nrect, height/nrect);
            }

            // RENDER rectangles
            for (i = 0; i < nrect; i++) {
                Rect rect = rects[i];
                rect.render(&map);
            }

            Rect tunnels[(nrect - 1) * 2];

            for (i= 0; i < nrect; i++) {
                Rect rect0 = rects[i];
                Rect rect1 = rects[i+1];

                Rect tunnel_x;
                Rect tunnel_y;

                int r0_x = rect0.center_x();
                int r0_y = rect0.center_y();
                int r1_x = rect1.center_x();
                int r1_y = rect1.center_y();

                if (r1_x > r0_x) {
                    tunnel_x.x0 = r0_x;
                    tunnel_x.x1 = r1_x;
                } else {
                    tunnel_x.x0 = r1_x;
                    tunnel_x.x1 = r0_x;
                }

                if (r1_y > r0_y) {
                    tunnel_y.y0 = r0_y;
                    tunnel_y.y1 = r1_y;
                } else {
                    tunnel_y.y0 = r1_y;
                    tunnel_y.y1 = r0_y;
                }

                tunnel_x.render(&map);
                tunnel_y.render(&map);
            }


        }

    public:
        Map(int w, int h)
            : width(w), height(h), map(std::vector<std::vector<char> >(w, std::vector<char>(h, '#')))
        { generate_maze(); }

        const int get_width() { return width; }
        const int get_height() { return height; }
        const char at(int x, int y) { return map[x][y]; }
        /* void set_at(char c, int x, int y) { map[x][y] = c; } */
};
