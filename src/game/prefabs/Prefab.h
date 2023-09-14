#pragma once

#include "engine/core/ecs/entt.h"
#include <engine/utils/vector2.h>

namespace core::ecs
{
    class Entity;
}

namespace prefabs
{
    void createCampfire(core::ecs::Entity &entity, utils::Vector2 &position);

    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position);
} // namespace prefabs
