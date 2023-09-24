#include "CampfireEntity.h"
#include "game/Inventory.h"
#include <engine/core/input.h>
#include <engine/core/ecs/Entity.h>

CampfireEntity::CampfireEntity()
{
}

void CampfireEntity::beginCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    if (collider.entity.compareTag("player"))
    {
        playerEntity = collider.entity;
    }
}

void CampfireEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    playerEntity = {};
}

bool CampfireEntity::onHandleInput([[maybe_unused]] core::Input *input)
{
    if (input->isMouseButtonPressed(SDL_BUTTON_RIGHT) && playerEntity)
    {
        // auto &inventory = playerEntity.findComponent<Inventory>();
        //  inventory.addItem()

        // this->entity.getScene()->destoryEntity(entity);
    }

    return false;
}

CampfireEntity::~CampfireEntity()
{
}