#pragma once

#include <assert.h>

class Point {
    public:
        int x, y;
        explicit Point(int x, int y) : x(x), y(y) {}
        Point operator-(Point p)  { return Point(x - p.x, y - p.x); }
        Point operator+(Point p)  { return Point(x + p.x, y + p.x); }
};

class Rect {
    public:
        int x0, y0, x1, y1;
        Rect() { }
        explicit Rect(int x0, int y0, int x1, int y1) {
            assert((x1 > x0) && (y1 > x0));
        }
};

int center_x(Rect r) { return ((r.x1 - r.x0) / 2) + r.x0; }
int center_y(Rect r) { return ((r.y1 - r.y0) / 2) + r.y0; }
Point center(Rect r) { return Point(center_x(r), center_y(r)); }
