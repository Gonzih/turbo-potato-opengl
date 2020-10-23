#pragma once

#include <bitset>
#include <memory>
#include <vector>
#include <array>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "../logging.hpp"
#include "registry_keys.hpp"

namespace ecs
{
    constexpr std::size_t maxComponents = 32;

    class Entity;
    class Component;
    class System;
    class Registry;

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
    protected:
        Entity* m_entity;
        std::shared_ptr<Registry> m_reg;
    public:
        Component() {  };
        virtual ~Component() {  };

        virtual void init() { }
        virtual void update() { }
        virtual void draw() { }

        void set_entity(Entity* e)
        { m_entity = e; }

        void set_reg(std::shared_ptr<Registry> e)
        { m_reg = e; }
    };

    class Entity
    {
    private:
        bool active = true;
        ComponentArray components;
        ComponentBitSet component_bit_set;
        std::shared_ptr<Registry> reg;
    public:
        Entity(std::shared_ptr<Registry> reg) : reg { reg } { };
        virtual ~Entity() {  };

        void init()
        { for (auto& c : components) if (c) c->init();  }

        void update()
        { for (auto& c : components) if (c) c->update();  }

        void draw()
        { for (auto& c : components) if (c) c->draw(); }

        bool is_active() const { return active; }
        void destroy() { active = false; }

        template <typename T>
        bool has_component() const
        {
            return component_bit_set.test(get_component_type_id<T>());
        }

        template <typename T>
        void assert_component(std::string label) const
        {
            std::string name = typeid(T).name();
            if (!has_component<T>()) {
                throw std::runtime_error("Entity needs " + name + " component to be present for " + label + " component");
            }
        }

        template <typename T, typename... TArgs>
        std::shared_ptr<T> add_component(TArgs&&... mArgs)
        {
            std::shared_ptr<T> c = std::make_shared<T>(mArgs...);
            c->set_entity(this);
            c->set_reg(reg);
            c->init();

            components[get_component_type_id<T>()] = c;
            component_bit_set.set(get_component_type_id<T>());

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
        std::shared_ptr<Registry> reg;
    public:
        System(std::shared_ptr<Registry> reg) : reg { reg } { };
        ~System() { };

        void update()
        {
            for (auto& e : entities) e->update();
        }

        void draw()
        {
            for (auto& e : entities) e->draw();
        }

        std::vector<std::shared_ptr<Entity>> get_entities() const
        { return entities; }

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
            std::shared_ptr<Entity> e = std::make_shared<Entity>(reg);
            entities.push_back(e);
            return e;
        };
    };

    inline size_t gen_reg_key()
    {
        static size_t id = -1;
        return ++id;
    }

    template <size_t k>
    inline size_t reg_key() noexcept
    {
        static size_t id = gen_reg_key();
        return id;
    }

    class Registry
    {
    private:
        std::vector<std::weak_ptr<Entity>> reg;
    public:
        Registry() = default;
        ~Registry() = default;

        template <size_t k>
        void save(std::weak_ptr<Entity> e)
        {
            reg_key<k>();
            reg.push_back(e);
        }

        template <size_t k>
        std::shared_ptr<Entity> find()
        {
            size_t id = reg_key<k>();
            if (auto p = reg[id].lock())
                return p;
            else
                throw std::runtime_error("Could not lock shared pointer for " + std::to_string(k));
        }

        template <size_t k, typename T>
        std::shared_ptr<T> component()
        {
            std::shared_ptr<Entity> p = find<k>();
            return p->get_component<T>();
        }
    };
};
