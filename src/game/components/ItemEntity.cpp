#include "ItemEntity.h"
#include <iostream>
#include <engine/core/ecs/Entity.h>
#include "game/Inventory.h"

void ItemEntity::onUpdate([[maybe_unused]] size_t delta)
{
    // auto &renderComponent = entity.findComponent<core::ecs::RenderComponent>();
    // std::cout << "update item entity" << std::endl;
}

void ItemEntity::beginCollision([[maybe_unused]] const core::ecs::Collision &collider)
{

    auto tag = collider.entity.tagName();
    std::cout << "collision with tag: " << tag << std::endl;
    if (!item)
    {
        std::cout << "no item defined" << std::endl;
        return;
    }
    activeCollisionEntity = collider.entity;
    Inventory &inventory = activeCollisionEntity.findComponent<Inventory>();
    switch (typeofItemDestruction)
    {
    case TypeOfItemDestruction::Automatic:
        inventory.addItem(item, 1);
        entity.getScene()->destoryEntity(entity);
        break;

    default:
        break;
    }
}
void ItemEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    activeCollisionEntity = {};
}

bool ItemEntity::onHandleInput(core::Input *input)
{

    if (input->isMouseButtonPressed(SDL_BUTTON_RIGHT) && activeCollisionEntity && item)
    {
        Inventory &inventory = activeCollisionEntity.findComponent<Inventory>();

        switch (typeofItemDestruction)
        {
        case TypeOfItemDestruction::RightClick:
            inventory.addItem(item, 1);
            entity.getScene()->destoryEntity(entity);

            return true;

        default:
            break;
        }
    }
    return false;
}

ItemEntity::ItemEntity() : core::ecs::ScriptableEntity()
{
}

ItemEntity::~ItemEntity()
{
}
void ItemEntity::setItem(const std::shared_ptr<Item> &item)
{
    this->item = item;
}

void ItemEntity::setTypeOfItemDestruction(TypeOfItemDestruction type)
{
    typeofItemDestruction = type;
}
