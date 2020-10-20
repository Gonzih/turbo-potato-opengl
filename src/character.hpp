#pragma once

#include <utility>
#include "geometry.hpp"

enum CharacterType {
    NotDefined,
    CharacterPlayer,
    CharacterNPC,
};

enum MovementDirection {
    None, Up, Down, Left, Right
};

class Character {
    private:
    public:
        Character() {};
        virtual const CharacterType type() { return CharacterType::NotDefined; }
};

class Player: public Character {
    private:
        Point pos;
    public:
        Player(int x, int y) : pos(x, y) {};

        void move_to(Point new_pos) {
            pos = new_pos;
        }

        void move(MovementDirection direction) {
            switch(direction) {
                case MovementDirection::Up:
                    pos.y -= 1;
                    break;
                case MovementDirection::Down:
                    pos.y += 1;
                    break;
                case MovementDirection::Left:
                    pos.x -= 1;
                    break;
                case MovementDirection::Right:
                    pos.x += 1;
                    break;
                case MovementDirection::None:
                    break;
            }
        }

        const CharacterType type() { return CharacterType::CharacterPlayer; }

        const Point get_pos() { return pos; };
        const int get_x() { return pos.x; };
        const int get_y() { return pos.y; };
};
