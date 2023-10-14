#pragma once

#include "game/Item.h"
#include <memory>

class Item;

namespace core
{
    namespace ecs
    {
        class Entity;
    } // namespace ecs

} // namespace core

namespace actions
{
    class ItemAction
    {
    private:
        /* data */
    public:
        virtual bool execute(std::shared_ptr<Item> &item, core::ecs::Entity &entity) = 0;
        ItemAction(/* args */){};
        virtual ~ItemAction(){};
    };

} // namespace actions
