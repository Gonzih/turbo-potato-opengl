#pragma once

#include <memory>

#include "geometry.hpp"

namespace ecs
{
    namespace components
    {
        class PositionComponent : public Component
        {
        private:
            Point pos;
        public:
            PositionComponent() : pos { 0, 0 } {};
            virtual ~PositionComponent() override {  };

            void move_to(Point new_pos)
            {
                pos = new_pos;
            }

            void move(MovementDirection direction)
            {
                switch(direction)
                {
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

            const Point get_pos() { return pos; };
            const int get_x() { return pos.x; };
            const int get_y() { return pos.y; };
        };

        class AsciiRenderComponent : public Component
        {
        private:
            std::wstring symbol;
        public:
            AsciiRenderComponent() : symbol { L"" } {  };
            virtual ~AsciiRenderComponent() override {  };

            void set_symbol(std::wstring s)
            { symbol = s; }

            const std::wstring get_symbol()
            { return symbol; }
        };
    }
};
