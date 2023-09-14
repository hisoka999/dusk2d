#include "CampfireEntity.h"

CampfireEntity::CampfireEntity()
{
}

void CampfireEntity::beginCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
}

void CampfireEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
}

bool CampfireEntity::onHandleInput([[maybe_unused]] core::Input *input)
{
    return false;
}

CampfireEntity::~CampfireEntity()
{
}