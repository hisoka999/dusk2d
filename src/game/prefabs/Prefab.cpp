#include "Prefab.h"
#include <engine/core/ecs/Entity.h>
#include <engine/graphics/TextureManager.h>
#include <engine/core/ecs/Component.h>
#include <map>
#include "game/components/CampfireEntity.h"
#include "game/components/TreeEntity.h"
#include "game/components/RockEntity.h"
#include "game/components/ItemEntity.h"
#include "game/GameMap.h"
#include "game/services/ItemService.h"
#include <engine/core/ecs/ScriptComponent.h>

namespace prefabs
{
    using namespace std::literals;

    void createCampfire(core::ecs::Entity &entity, utils::Vector2 &position)
    {

        core::ecs::Transform &transform = entity.addComponent<core::ecs::Transform>();
        transform.position = position;
        auto &textureMap = graphics::TextureManager::Instance().loadTextureMap("images/campfire.json");

        transform.width = textureMap->getChildTexture("animation1")->getRect().width * 2;
        transform.height = textureMap->getChildTexture("animation1")->getRect().height * 2;
        utils::Vector2 startPos = {.0f, .0f};
        graphics::TextureMapAnimation animation(startPos, textureMap);
        animation.setRepeating(-1);
        animation.createFrame<std::string>(startPos, 100, std::string{"animation1"});
        animation.createFrame<std::string>(startPos, 100, std::string{"animation2"});
        animation.createFrame<std::string>(startPos, 100, std::string{"animation3"});
        animation.createFrame<std::string>(startPos, 100, std::string{"animation4"});
        animation.play();
        entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animation);

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Static;
        rb2d.FixedRotation = true;
        collider.Density = 1.f;
        collider.Friction = 0;
        collider.Restitution = 0.0;
        collider.RestitutionThreshold = 0.0;
        collider.Offset = {0.f, 0.5f};
        collider.Size = {1.f, 1.f};

        core::ecs::addScriptComponent<CampfireEntity>(entity);
        core::ecs::addScriptComponent<ItemEntity>(entity);

        auto &itemScript = core::ecs::addScriptComponent<ItemEntity>(entity);
        ((ItemEntity *)itemScript.Instance)->setTypeOfItemDestruction(TypeOfItemDestruction::RightClick);
        auto item = services::ItemService::Instance().getItemByName("Campfire");
        ((ItemEntity *)itemScript.Instance)->setItem(item);
    }

    void createTree(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &treeTexture = graphics::TextureManager::Instance().loadTexture("images/trees/pine_tree.png");

        core::ecs::Transform transform;
        transform.position = position - utils::Vector2{54.f, 128.f};
        transform.width = treeTexture->getWidth();
        transform.height = treeTexture->getHeight();
        entity.addComponent<core::ecs::Transform>(transform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Static;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {2.0f, 4.1f};
        collider.Size = {0.5f, 0.5f};
        // collider.Friction = 0;
        // collider.RestitutionThreshold = 0;
        entity.addComponent<core::ecs::RenderComponent>(treeTexture);

        core::ecs::addScriptComponent<TreeEntity>(entity);
    }

    void createRock(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &rockTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/rock.json");

        core::ecs::Transform transform;
        transform.position = position;
        transform.width = TILE_SIZE / 2;
        transform.height = TILE_SIZE / 2;
        entity.addComponent<core::ecs::Transform>(transform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Static;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.5f, 0.5f};
        collider.Size = {1.f, 1.0f};
        // collider.Friction = 0;
        // collider.RestitutionThreshold = 0;
        entity.addComponent<core::ecs::RenderComponent>(rockTextureMap->getChildTexture("rock"));

        core::ecs::addScriptComponent<RockEntity>(entity);
    }

    void createWood(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("wood");
        core::ecs::Transform itemTransform;
        itemTransform.position = position;
        itemTransform.width = childTexture->getRect().width;
        itemTransform.height = childTexture->getRect().height;
        entity.addComponent<core::ecs::Transform>(itemTransform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Kinematic;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.25f, 0.25f};
        collider.Size = {0.25, 0.25};
        collider.Density = 0.5;
        // collider.Friction = 0;
        // collider.RestitutionThreshold = 0;
        entity.addComponent<core::ecs::RenderComponent>(childTexture);

        auto &script = core::ecs::addScriptComponent<ItemEntity>(entity);
        auto item = std::make_shared<Item>(1, ItemType::WOOD, "Wood", "wood", "");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    void createStone(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("stone");
        core::ecs::Transform itemTransform;
        itemTransform.position = position;
        itemTransform.width = childTexture->getRect().width;
        itemTransform.height = childTexture->getRect().height;
        entity.addComponent<core::ecs::Transform>(itemTransform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Kinematic;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.25f, 0.25f};
        collider.Size = {0.25, 0.25};
        collider.Density = 0.5;

        entity.addComponent<core::ecs::RenderComponent>(childTexture);

        auto &script = core::ecs::addScriptComponent<ItemEntity>(entity);
        auto item = std::make_shared<Item>(2, ItemType::STONE, "Stone", "stone", "");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    void createApple(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("apple");
        core::ecs::Transform itemTransform;
        itemTransform.position = position;
        itemTransform.width = childTexture->getRect().width;
        itemTransform.height = childTexture->getRect().height;
        entity.addComponent<core::ecs::Transform>(itemTransform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Kinematic;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.25f, 0.25f};
        collider.Size = {0.25, 0.25};
        collider.Density = 0.5;

        entity.addComponent<core::ecs::RenderComponent>(childTexture);

        auto &script = core::ecs::addScriptComponent<ItemEntity>(entity);
        auto item = services::ItemService::Instance().getItemByName("Apple");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    static std::map<std::string, std::function<void(core::ecs::Entity &, utils::Vector2 &)>> prefabList = {
        {"campfire"s, createCampfire}, {"tree"s, createTree}, {"rock"s, createRock}, {"wood"s, createWood}, {"stone"s, createStone}, {"apple"s, createApple}};

    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position)
    {
        if (prefabList.count(prefabName))
        {
            prefabList.at(prefabName).operator()(entity, position);
        }
    }
} // namespace prefabs
