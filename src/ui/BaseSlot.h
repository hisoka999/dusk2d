#pragma once
#include "game/Item.h"

namespace UI
{
    class BaseSlot
    {
    private:
        /* data */
    public:
        virtual int getAmount() = 0;
        virtual std::shared_ptr<Item> &getItem() = 0;
        virtual void updateSlot(std::shared_ptr<Item> item, int amount) = 0;

        BaseSlot(/* args */) {}
        ~BaseSlot() {}
    };
} // namespace UI
