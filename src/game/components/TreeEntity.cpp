#include "TreeEntity.h"
#include <engine/graphics/TextureManager.h>
#include "ItemEntity.h"
#include <engine/core/input.h>
#include <engine/core/ecs/Entity.h>
#include <engine/core/ecs/ScriptComponent.h>
#include "game/prefabs/Prefab.h"
#include <random>
#include "game/Inventory.h"

TreeEntity::TreeEntity()
{
}

TreeEntity::~TreeEntity()
{
}

void TreeEntity::onClick(int button)
{
    if (button == SDL_BUTTON_LEFT)
    {
        auto player = getEntity().getScene()->findEntityByName("player");
        auto &inventory = player->findComponent<Inventory>();
        auto slot = inventory.getSelectedHotbarSlot();
        if (slot.item && slot.item->getItemSubType() == ItemSubType::AXE)
        {
            auto &transform = entity.findComponent<core::ecs::Transform>();

            std::random_device device;
            std::mt19937 gen(device());
            std::uniform_real_distribution<double> posDist(-16.0, 16.0);
            std::uniform_int_distribution<int> numItems(1, 5);

            std::uniform_int_distribution<int> itemChance(1, 100);
            for (int i = 1; i <= numItems(gen); ++i)
            {
                auto position = transform.position + utils::Vector2{transform.width / 2.0f, float(transform.height)} + utils::Vector2(posDist(gen), posDist(gen));
                int chance = itemChance(gen);
                if (chance > 15)
                {
                    auto itemEntity = entity.getScene()->createEntity("wood");
                    prefabs::instantiateFromPrefab(itemEntity, "wood", position);
                }
                else
                {
                    auto itemEntity = entity.getScene()->createEntity("apple");
                    prefabs::instantiateFromPrefab(itemEntity, "apple", position);
                }
            }

            entity.getScene()->destoryEntity(entity);
        }
    }
}

bool TreeEntity::onHandleInput(core::Input *input)
{

    if (input->isKeyDown("INPUT") && playerEntity.compareTag("player"))
    {
        auto &inventory = playerEntity.findComponent<Inventory>();
        auto slot = inventory.getSelectedHotbarSlot();
        if (slot.item && slot.item->getItemSubType() == ItemSubType::AXE)
        {
            auto &transform = entity.findComponent<core::ecs::Transform>();

            std::random_device device;
            std::mt19937 gen(device());
            std::uniform_real_distribution<double> posDist(-16.0, 16.0);
            std::uniform_int_distribution<int> numItems(1, 5);

            std::uniform_int_distribution<int> itemChance(1, 100);
            for (int i = 1; i <= numItems(gen); ++i)
            {
                auto position = transform.position + utils::Vector2{transform.width / 2.0f, float(transform.height)} + utils::Vector2(posDist(gen), posDist(gen));
                int chance = itemChance(gen);
                if (chance > 15)
                {
                    auto itemEntity = entity.getScene()->createEntity("wood");
                    prefabs::instantiateFromPrefab(itemEntity, "wood", position);
                }
                else
                {
                    auto itemEntity = entity.getScene()->createEntity("apple");
                    prefabs::instantiateFromPrefab(itemEntity, "apple", position);
                }
            }

            entity.getScene()->destoryEntity(entity);
            return true;
        }
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
