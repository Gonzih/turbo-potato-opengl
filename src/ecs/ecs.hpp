#pragma once

#include <bitset>
#include <memory>
#include <vector>
#include <array>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "../logging.hpp"

namespace ecs
{
    constexpr std::size_t maxComponents = 32;

    class Entity;
    class Component;
    class System;
    class Group;

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
    public:
        Component() {  };
        virtual ~Component() {  };

        virtual void init() { }
        virtual void update() { }
        virtual void draw() { }

        void set_entity(Entity* e)
        { m_entity = e; }
    };

    class Entity
    {
    private:
        bool m_active = true;
        ComponentArray m_components;
        ComponentBitSet m_component_bit_set;
    public:
        Entity() { };
        virtual ~Entity() {  };

        void init()
        { for (auto& c : m_components) if (c) c->init();  }

        void update()
        { for (auto& c : m_components) if (c) c->update();  }

        void draw()
        { for (auto& c : m_components) if (c) c->draw(); }

        bool is_active() const { return m_active; }
        void destroy() { m_active = false; }

        template <typename T>
        bool has_component() const
        {
            return m_component_bit_set.test(get_component_type_id<T>());
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
            c->init();

            m_components[get_component_type_id<T>()] = c;
            m_component_bit_set.set(get_component_type_id<T>());

            return c;
        }

        template <typename T>
        std::shared_ptr<T> get_component() {
            auto ptr = m_components[get_component_type_id<T>()];
            return std::static_pointer_cast<T>(ptr);
        }
    };

    class Group
    {
    private:
        std::vector<std::shared_ptr<Entity>> m_entities;
    public:
        Group() { };
        ~Group() { };

        void update()
        {
            for (auto& e : m_entities) e->update();
        }

        void draw()
        {
            for (auto& e : m_entities) e->draw();
        }

        void destroy_all()
        {
            for (auto& e : m_entities) e->destroy();
        }

        void collect_garbage()
        {
            m_entities.erase(
                    std::remove_if(
                        m_entities.begin(), m_entities.end(),
                        [](const std::shared_ptr<Entity> e) { return !e->is_active(); }),
                    m_entities.end());
        }

        std::shared_ptr<Entity> add_entity()
        {
            std::shared_ptr<Entity> e = std::make_shared<Entity>();
            m_entities.push_back(e);
            return e;
        };
    };

    class System
    {
    private:
        std::vector<std::shared_ptr<Group>> m_groups;
    public:
        System() { };
        ~System() { };

        void update()
        {
            for (auto& g : m_groups) g->update();
        }

        void draw()
        {
            for (auto& g : m_groups) g->draw();
        }

        void collect_garbage()
        {
            for (auto& g : m_groups) g->collect_garbage();
        }

        std::shared_ptr<Group> add_group()
        {
            std::shared_ptr<Group> g = std::make_shared<Group>();
            m_groups.push_back(g);
            return g;
        };
    };
};
