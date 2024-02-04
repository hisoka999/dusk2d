#pragma once

#include <engine/utils/vector2.h>
#include <map>
#include "engine/core/ecs/entt.h"

namespace core::ecs
{
    class Entity;
}

namespace prefabs
{
    typedef std::map<std::string, std::string> ArgsMap;
    // void createCampfire(core::ecs::Entity &entity, utils::Vector2 &position);

    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position);

    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position,
                               ArgsMap &args);

} // namespace prefabs
