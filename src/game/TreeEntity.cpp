#include "TreeEntity.h"
#include <engine/graphics/TextureManager.h>
#include "ItemEntity.h"
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
        auto itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("wood");
        auto itemEntity = entity.getScene()->createEntity("wood");
        core::ecs::Transform itemTransform;
        itemTransform.position = transform.position + utils::Vector2{transform.width / 2.0f, float(transform.height)};
        itemTransform.width = childTexture->getRect().width;
        itemTransform.height = childTexture->getRect().height;
        itemEntity.addComponent<core::ecs::Transform>(itemTransform);
        auto &rb2d = itemEntity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Kinematic;

        auto &collider = itemEntity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.25f, 0.25f};
        collider.Size = {0.25, 0.25};
        collider.Density = 0.5;
        // collider.Friction = 0;
        // collider.RestitutionThreshold = 0;
        itemEntity.addComponent<core::ecs::RenderComponent>(childTexture);

        auto &script = itemEntity.addComponent<core::ecs::ScriptComponent>();
        script.Bind<ItemEntity>();
        script.Instance = script.InstantiateScript();
        script.Instance->setEntity(itemEntity);
        auto item = std::make_shared<Item>(1, ItemType::WOOD, "Wood", "wood");
        ((ItemEntity *)script.Instance)->setItem(item);

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
