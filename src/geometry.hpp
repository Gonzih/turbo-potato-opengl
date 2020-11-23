#pragma once

#include <functional>
#include <utility>
#include <cstdint>
#include <string>
#include <assert.h>

class Vector2D {
    public:
        int x, y;

        Vector2D() {};
        explicit Vector2D(std::pair<int, int> v) : x { v.first } , y { v.second } {}
        explicit Vector2D(int x, int y) : x { x }, y { y } {}
        virtual ~Vector2D() {  };

        Vector2D operator-(Vector2D p) const { return Vector2D(x - p.x, y - p.y); }
        Vector2D operator+(Vector2D p) const { return Vector2D(x + p.x, y + p.y); }
        Vector2D operator/(int v) const { return Vector2D(x/v, y/v); }
        Vector2D operator*(int v) const { return Vector2D(x*v, y*v); }
        bool operator==(const Vector2D& p) const { return (x == p.x) && (y == p.y); }

        std::string to_string() const
        {
            return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
        }
};

class Rect {
    public:
        int x0, y0, x1, y1;
        Rect() { }
        explicit Rect(int x0, int y0, int x1, int y1) {
            assert((x1 > x0) && (y1 > x0));
        }
};

inline int center_x(Rect r) { return ((r.x1 - r.x0) / 2) + r.x0; }
inline int center_y(Rect r) { return ((r.y1 - r.y0) / 2) + r.y0; }
inline Vector2D center(Rect r) { return Vector2D(center_x(r), center_y(r)); }

namespace std {
  template<> struct hash<Vector2D> {
    // hash fn taken from https://stackoverflow.com/questions/20590656/error-for-hash-function-of-pair-of-ints
    size_t operator()(Vector2D const& p) const {
        uintmax_t hash = std::hash<int>{}(p.x);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<int>{}(p.y);
        return std::hash<uintmax_t>{}(hash);
    }
  };
}
