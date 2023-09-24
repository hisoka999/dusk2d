#pragma once

#include <engine/core/ecs/Component.h>
#include "game/Item.h"

enum class TypeOfItemDestruction
{
    Automatic,
    RightClick,
    Axe,
    PickAxe
};

class ItemEntity : public core::ecs::ScriptableEntity
{
private:
    std::shared_ptr<Item> item = nullptr;
    core::ecs::Entity activeCollisionEntity;
    TypeOfItemDestruction typeofItemDestruction = TypeOfItemDestruction::Automatic;

public:
    void setItem(const std::shared_ptr<Item> &item);
    void setTypeOfItemDestruction(TypeOfItemDestruction type);
    void onUpdate(size_t delta) override;
    void beginCollision([[maybe_unused]] const core::ecs::Collision &collider) override;
    void endCollision([[maybe_unused]] const core::ecs::Collision &collider) override;

    bool onHandleInput(core::Input *input) override;

    ItemEntity();
    virtual ~ItemEntity();
};