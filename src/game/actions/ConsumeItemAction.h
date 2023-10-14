#pragma once

#include "ItemAction.h"

namespace actions
{
    class ConsumeItemAction : ItemAction
    {
    private:
        /* data */
    public:
        virtual bool execute(std::shared_ptr<Item> &item, core::ecs::Entity &entity);

        ConsumeItemAction(/* args */);
        ~ConsumeItemAction();
    };

} // namespace actions
