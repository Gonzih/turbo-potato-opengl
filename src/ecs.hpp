#pragma once

#include <bitset>
#include <memory>
#include <vector>
#include <array>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "geometry.hpp"

enum class MovementDirection {
    None, Up, Down, Left, Right
};

namespace ecs
{
    constexpr std::size_t maxComponents = 32;

    class Entity;
    class Component;
    class System;

    using ComponentTypeID = size_t;

    inline ComponentTypeID gen_component_type_id()
    {
        static ComponentTypeID compID = 0;
        return ++compID;
    }

    template <typename T>
    inline ComponentTypeID get_component_type_id() noexcept
    {
        static ComponentTypeID id = gen_component_type_id();
        return id;
    }

    using ComponentArray = std::array<std::shared_ptr<Component>, maxComponents>;
    using ComponentBitSet = std::bitset<maxComponents>;

    class Component
    {
    private:
    public:
        Entity* entity;

        Component() {  };
        virtual ~Component() {  };

        virtual void init() { }
        virtual void update() { }
        virtual void draw() { }
    };

    class Entity
    {
    private:
        bool active = true;
        ComponentArray components;
        ComponentBitSet component_bit_set;

    public:
        Entity() {  };
        virtual ~Entity() {  };

        void init()
        {

        }

        void update()
        {
            for (auto& c : components) c->update();
        }

        void draw()
        {
            for (auto& c : components) c->draw();
        }

        bool is_active() const { return active; }
        void destroy() { active = false; }

        template <typename T>
        bool has_component() const
        {
            return component_bit_set.test(get_component_type_id<T>());
        }

        template <typename T>
        std::shared_ptr<T> add_component()
        {

            std::shared_ptr<T> c = std::make_shared<T>();
            c->entity = this;
            components[get_component_type_id<T>()] = c;
            component_bit_set.set(get_component_type_id<T>());
            c->init();

            return c;
        }

        template <typename T>
        std::shared_ptr<T> get_component() {
            auto ptr = components[get_component_type_id<T>()];
            return std::static_pointer_cast<T>(ptr);
        }


    };

    class System
    {
    private:
        std::vector<std::shared_ptr<Entity>> entities;
    public:
        void update()
        {
            for (auto& e : entities) e->update();
        }
        void draw()
        {
            for (auto& e : entities) e->draw();
        }
        void collect_garbage()
        {
            entities.erase(
                    std::remove_if(
                        entities.begin(), entities.end(),
                        [](const std::shared_ptr<Entity> e) { return !e->is_active(); }),
                    entities.end());
        }

        std::shared_ptr<Entity> add_entity()
        {
            std::shared_ptr<Entity> e = std::make_shared<Entity>();
            entities.push_back(e);
            return e;
        };
    };

    namespace components
    {
        // Position component
        // Handle input component
        /* class */

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
    }
};
