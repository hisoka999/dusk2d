#include "RockEntity.h"
#include <engine/graphics/TextureManager.h>
#include "ItemEntity.h"
#include <engine/core/input.h>
#include <engine/core/ecs/Entity.h>
#include <engine/core/ecs/ScriptComponent.h>
#include <random>
#include "game/prefabs/Prefab.h"

RockEntity::RockEntity() : minItemSpawn(0), maxItemSpawn(2)
{
}

RockEntity::~RockEntity()
{
}
void RockEntity::onClick(int button) {
    if(button == SDL_BUTTON_LEFT) {
        auto &transform = entity.findComponent<core::ecs::Transform>();

        std::random_device device;
        std::mt19937 gen(device());
        std::uniform_real_distribution<double> posDist(0.0, 16.0);
        std::uniform_int_distribution<int> numItems(minItemSpawn, maxItemSpawn);

        for (int i = 1; i <= numItems(gen); ++i)
        {
            auto position = transform.position + utils::Vector2(posDist(gen), posDist(gen));
            auto itemEntity = entity.getScene()->createEntity("stone");

            prefabs::instantiateFromPrefab(itemEntity, "stone", position);
        }

        entity.getScene()->destoryEntity(entity);
    }
}
bool RockEntity::onHandleInput(core::Input *input)
{
    if (input->isKeyDown("INPUT") && playerEntity.compareTag("player"))
    {
        auto &transform = entity.findComponent<core::ecs::Transform>();

        std::random_device device;
        std::mt19937 gen(device());
        std::uniform_real_distribution<double> posDist(0.0, 16.0);
        std::uniform_int_distribution<int> numItems(minItemSpawn, maxItemSpawn);

        for (int i = 1; i <= numItems(gen); ++i)
        {
            auto position = transform.position + utils::Vector2(posDist(gen), posDist(gen));
            auto itemEntity = entity.getScene()->createEntity("stone");

            prefabs::instantiateFromPrefab(itemEntity, "stone", position);
        }

        entity.getScene()->destoryEntity(entity);
        return true;
    }
    return false;
}

void RockEntity::beginCollision(const core::ecs::Collision &collider)
{
    if (collider.entity.compareTag("player"))
    {
        playerEntity = collider.entity;
    }
}

void RockEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    playerEntity = core::ecs::Entity();
}

void RockEntity::setMinItemSpawn(int spawn)
{
    minItemSpawn = spawn;
}

void RockEntity::setMaxItemSpawn(int spawn)
{
    maxItemSpawn = spawn;
}
