#pragma once

#include <memory>

#include "ecs.hpp"

namespace ecs
{
    namespace components
    {
        class AsciiRenderComponent : public Component
        {
        private:
            std::wstring symbol;
        public:
            AsciiRenderComponent() : symbol { L"" } {  };
            AsciiRenderComponent(std::wstring s) : symbol { s } {  };
            virtual ~AsciiRenderComponent() override {  };

            void set_symbol(std::wstring s)
            { symbol = s; }

            const std::wstring get_symbol()
            { return symbol; }
        };
    }
};
