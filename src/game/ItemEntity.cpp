#include "ItemEntity.h"
#include <iostream>
#include <engine/core/ecs/Entity.h>
#include "Inventory.h"

void ItemEntity::onUpdate([[maybe_unused]] size_t delta)
{
    // auto &renderComponent = entity.findComponent<core::ecs::RenderComponent>();
    // std::cout << "update item entity" << std::endl;
}

void ItemEntity::beginCollision([[maybe_unused]] const core::ecs::Collision &collider)
{

    auto tag = collider.entity.tagName();
    std::cout << "collision with tag: " << tag << std::endl;
    auto e = collider.entity;
    Inventory &inventory = e.findComponent<Inventory>();
    inventory.addItem(type, 1);

    entity.getScene()->destoryEntity(entity);
}

ItemEntity::ItemEntity() : core::ecs::ScriptableEntity()
{
}

ItemEntity::~ItemEntity()
{
}

void ItemEntity::setItemType(ItemType type)
{
    this->type = type;
}
