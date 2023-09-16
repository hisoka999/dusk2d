#pragma once

#include <engine/core/ecs/Component.h>
#include "game/Item.h"

class ItemEntity : public core::ecs::ScriptableEntity
{
private:
    std::shared_ptr<Item> item = nullptr;

public:
    void setItem(const std::shared_ptr<Item> &item);
    void onUpdate(size_t delta) override;
    void beginCollision([[maybe_unused]] const core::ecs::Collision &collider);

    ItemEntity();
    virtual ~ItemEntity();
};