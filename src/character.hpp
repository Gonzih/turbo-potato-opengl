#include <utility>

enum CharacterType {
    NotDefined,
    CharacterPlayer,
    CharacterNPC,
};

enum MovementDirection {
    Up, Down, Left, Right
};

class Character {
    private:
    public:
        Character() {};
        const CharacterType type() { return CharacterType::NotDefined; }
};

class Player: public Character {
    private:
        std::pair<int, int> pos;
    public:
        Player(int x, int y) : pos(x, y) {};

        void move_to(std::pair<int, int> new_pos) {
            pos = new_pos;
        }

        void move(MovementDirection direction) {
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
            }
        }

        const CharacterType type() { return CharacterType::CharacterPlayer; }

        const std::pair<int, int> get_pos() { return pos; };
        const int get_x() { return pos.first; };
        const int get_y() { return pos.second; };
};
