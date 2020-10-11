#include <vector>

class Map {
    private:
        int width;
        int height;
        std::vector<std::vector<char>> map;
    public:
        Map(int w, int h)
            : width(w), height(h), map(std::vector<std::vector<char>>(w, std::vector<char>(h, '.')))
        { }

        int get_width() { return width; }
        int get_height() { return height; }
        char at(int x, int y) { return map[x][y]; }
};
