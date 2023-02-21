#pragma once

#include <engine/core/ecs/Component.h>
#include "ItemType.h"

class ItemEntity : public core::ecs::ScriptableEntity
{
private:
    ItemType type;

public:
    void setItemType(ItemType type);
    void onUpdate(size_t delta) override;
    void beginCollision([[maybe_unused]] const core::ecs::Collision &collider);

    ItemEntity();
    virtual ~ItemEntity();
};