#include "TreeEntity.h"
#include <engine/graphics/TextureManager.h>
#include "ItemEntity.h"
#include <engine/core/input.h>
#include <engine/core/ecs/Entity.h>
#include <engine/core/ecs/ScriptComponent.h>
#include "game/prefabs/Prefab.h"
#include <random>

TreeEntity::TreeEntity()
{
}

TreeEntity::~TreeEntity()
{
}

bool TreeEntity::onHandleInput(core::Input *input)
{

    if (input->isKeyDown("INPUT") && playerEntity.compareTag("player"))
    {
        auto &transform = entity.findComponent<core::ecs::Transform>();

        std::random_device device;
        std::mt19937 gen(device());
        std::uniform_real_distribution<double> posDist(-16.0, 16.0);
        std::uniform_int_distribution<int> numItems(1, 5);
        for (int i = 1; i <= numItems(gen); ++i)
        {
            auto position = transform.position + utils::Vector2{transform.width / 2.0f, float(transform.height)} + utils::Vector2(posDist(gen), posDist(gen));
            auto itemEntity = entity.getScene()->createEntity("wood");

            prefabs::instantiateFromPrefab(itemEntity, "wood", position);
        }

        entity.getScene()->destoryEntity(entity);
        return true;
    }
    return false;
}

void TreeEntity::beginCollision(const core::ecs::Collision &collider)
{

    if (collider.entity.compareTag("player"))
    {
        playerEntity = collider.entity;
    }
}

void TreeEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    playerEntity = core::ecs::Entity();
}
