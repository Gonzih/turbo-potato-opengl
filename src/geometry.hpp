#pragma once

#include <functional>
#include <utility>
#include <cstdint>
#include <assert.h>

class Point {
    public:
        int x, y;

        Point() {};
        explicit Point(int x, int y) : x(x), y(y) {}
        virtual ~Point() {  };

        Point operator-(Point p)  { return Point(x - p.x, y - p.x); }
        Point operator+(Point p)  { return Point(x + p.x, y + p.x); }
        bool operator==(const Point& p) const { return (x == p.x) && (y == p.y); }
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

namespace std {
  template<> struct hash<Point> {
    // hash fn taken from https://stackoverflow.com/questions/20590656/error-for-hash-function-of-pair-of-ints
    size_t operator()(Point const& p) const {
        uintmax_t hash = std::hash<int>{}(p.x);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<int>{}(p.y);
        return std::hash<uintmax_t>{}(hash);
    }
  };
}
