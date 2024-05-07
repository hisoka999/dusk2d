#include "Prefab.h"
#include <charconv>
#include <engine/core/ecs/Component.h>
#include <engine/core/ecs/Entity.h>
#include <engine/core/ecs/ScriptComponent.h>
#include <engine/graphics/TextureManager.h>
#include <map>
#include "game/GameMap.h"
#include "game/Inventory.h"
#include "game/components/AnimalComponent.h"
#include "game/components/Character.h"
#include "game/components/ChestComponent.h"
#include "game/components/CraftingEntity.h"
#include "game/components/ItemEntity.h"
#include "game/components/RockEntity.h"
#include "game/components/TreeEntity.h"
#include "game/services/ItemService.h"

namespace prefabs
{
    using namespace std::literals;

    void createCampfire(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {

        core::ecs::Transform &transform = entity.addComponent<core::ecs::Transform>();
        transform.position = position;
        auto &textureMap = graphics::TextureManager::Instance().loadTextureMap("images/campfire.json");

        transform.width = textureMap->getChildTexture("animation1")->getRect().width * 2;
        transform.height = textureMap->getChildTexture("animation1")->getRect().height * 2;
        utils::Vector2 startPos = {.0f, .0f};
        graphics::TextureMapAnimator animator;
        {
            graphics::TextureMapAnimation animation(startPos, textureMap);

            animation.setRepeating(-1);
            animation.createFrame<std::string>(startPos, 100, std::string{"animation1"});
            animation.createFrame<std::string>(startPos, 100, std::string{"animation2"});
            animation.createFrame<std::string>(startPos, 100, std::string{"animation3"});
            animation.createFrame<std::string>(startPos, 100, std::string{"animation4"});
            animator.addAnimation("active", animation);
        }
        {
            graphics::TextureMapAnimation animation(startPos, textureMap);

            animation.setRepeating(-1);
            animation.createFrame<std::string>(startPos, 100, std::string{"idle"});
            animator.addAnimation("idle", animation);
        }
        animator.setCurrentAnimation("idle");
        entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animator);

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Static;
        rb2d.FixedRotation = true;
        collider.Density = 1.f;
        collider.Friction = 0;
        collider.Restitution = 0.0;
        collider.RestitutionThreshold = 0.0;
        collider.Offset = {.5f, 1.5f};
        collider.Size = {1.f, 1.f};

        entity.addComponent<Inventory>();

        core::ecs::addScriptComponent<CraftingEntity>(entity);

        auto &itemScript = core::ecs::addScriptComponent<ItemEntity>(entity);
        ((ItemEntity *)itemScript.Instance)->setTypeOfItemDestruction(TypeOfItemDestruction::RightClick);
        auto item = services::ItemService::Instance().getItemByName("Campfire");
        ((ItemEntity *)itemScript.Instance)->setItem(item);
    }

    void createFurnace(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {

        core::ecs::Transform &transform = entity.addComponent<core::ecs::Transform>();
        transform.position = position;
        auto &textureMap = graphics::TextureManager::Instance().loadTextureMap("images/furnace.json");

        transform.width = textureMap->getChildTexture("animation1")->getRect().width;
        transform.height = textureMap->getChildTexture("animation1")->getRect().height;
        utils::Vector2 startPos = {.0f, .0f};
        graphics::TextureMapAnimator animator;
        {
            graphics::TextureMapAnimation animation(startPos, textureMap);

            animation.setRepeating(-1);
            animation.createFrame<std::string>(startPos, 300, std::string{"animation1"});
            animation.createFrame<std::string>(startPos, 300, std::string{"animation2"});
            animation.createFrame<std::string>(startPos, 300, std::string{"animation3"});
            animator.addAnimation("active", animation);
        }
        {
            graphics::TextureMapAnimation animation(startPos, textureMap);

            animation.setRepeating(-1);
            animation.createFrame<std::string>(startPos, 100, std::string{"idle"});
            animator.addAnimation("idle", animation);
        }
        animator.setCurrentAnimation("idle");
        entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animator);

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Static;
        rb2d.FixedRotation = true;
        collider.Density = 1.f;
        collider.Friction = 0;
        collider.Restitution = 0.0;
        collider.RestitutionThreshold = 0.0;
        collider.Offset = {1.f, 2.0f};
        collider.Size = {2.f, 1.f};

        entity.addComponent<Inventory>();

        auto &craftingScript = core::ecs::addScriptComponent<CraftingEntity>(entity);
        static_cast<CraftingEntity *>(craftingScript.Instance)->setRecipeTarget(RecipeTarget::FURNACE);

        auto &itemScript = core::ecs::addScriptComponent<ItemEntity>(entity);
        ((ItemEntity *)itemScript.Instance)->setTypeOfItemDestruction(TypeOfItemDestruction::RightClick);
        auto item = services::ItemService::Instance().getItemByName("Furnace");
        ((ItemEntity *)itemScript.Instance)->setItem(item);
    }

    void createTree(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
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

    void createMountain(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &rockTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/mountain.json");

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

    void createRock(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &rockTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/rocks.json");

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

        auto component = static_cast<RockEntity *>(core::ecs::addScriptComponent<RockEntity>(entity).Instance);
        component->setMinItemSpawn(1);
        component->setMaxItemSpawn(1);
    }

    void createWood(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
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
        auto item = services::ItemService::Instance().getItemByName("Wood");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    void createStone(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
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
        auto item = services::ItemService::Instance().getItemByName("Stone");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    void createIronOre(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("iron_ore");
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
        auto item = services::ItemService::Instance().getItemByName("Iron Ore");
        ((ItemEntity *)script.Instance)->setItem(item);
    }
    void createSilverOre(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("silver_ore");
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
        auto item = services::ItemService::Instance().getItemByName("Silver Ore");
        ((ItemEntity *)script.Instance)->setItem(item);
    }
    void createGoldOre(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("gold_ore");
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
        auto item = services::ItemService::Instance().getItemByName("Gold Ore");
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    void createIron(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto item = services::ItemService::Instance().getItemByName("Iron Bar");
        auto childTexture = itemTextureMap->getChildTexture(item->getSubTextureName());
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

        ((ItemEntity *)script.Instance)->setItem(item);
    }
    void createSilver(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto item = services::ItemService::Instance().getItemByName("Silver Bar");
        auto childTexture = itemTextureMap->getChildTexture(item->getSubTextureName());
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
        ((ItemEntity *)script.Instance)->setItem(item);
    }
    void createGold(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto item = services::ItemService::Instance().getItemByName("Gold Bar");
        auto childTexture = itemTextureMap->getChildTexture(item->getSubTextureName());
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

        ((ItemEntity *)script.Instance)->setItem(item);
    }
    void createCoal(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture("coal");
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
        auto item = services::ItemService::Instance().getItemByName("Coal");
        ((ItemEntity *)script.Instance)->setItem(item);
    }


    void addAnimation(const std::string &direction, core::ecs::TextureMapAnimationRenderComponent &component,
                      const std::shared_ptr<graphics::TextureMap> textureMap)
    {
        if (component.animator.hasAnimation(direction))
        {
            component.animator.setCurrentAnimation(direction);
            return;
        }

        utils::Vector2 startPos = {0, 0};
        graphics::TextureMapAnimation animation(startPos, textureMap);
        animation.setRepeating(-1);
        for (int i = 1; i <= 4; i++)
        {
            std::string frame = direction + std::to_string(i);
            animation.createFrame<std::string>(startPos, 100, frame);
        }

        component.animator.addAnimation(direction, animation);
        component.animator.setCurrentAnimation(direction);
    }

    void createAnimal(core::ecs::Entity &entity, utils::Vector2 &position, ArgsMap &args)
    {
        auto name = args["name"];
        float offsetX = utils::to_float(args["offset_x"]);
        float offsetY = utils::to_float(args["offset_y"]);

        auto offset = utils::Vector2(offsetX, offsetY);
        core::ecs::Transform &transform = entity.addComponent<core::ecs::Transform>();
        transform.position = position;
        auto &textureMap = graphics::TextureManager::Instance().loadTextureMap("images/animals/" + name + ".json");

        transform.width = textureMap->getChildTexture("left1")->getRect().width;
        transform.height = textureMap->getChildTexture("left1")->getRect().height;
        utils::Vector2 startPos = {.0f, .0f};
        graphics::TextureMapAnimator animator;

        auto &animatorComponent = entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animator);
        addAnimation("left", animatorComponent, textureMap);
        addAnimation("right", animatorComponent, textureMap);
        addAnimation("up", animatorComponent, textureMap);
        addAnimation("down", animatorComponent, textureMap);

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Dynamic;
        rb2d.FixedRotation = true;
        collider.Density = 1.f;
        collider.Friction = 0;
        collider.Restitution = 0.0;
        collider.RestitutionThreshold = 0.0;
        collider.Offset = offset;
        collider.Size = {1.f, 1.f};

        auto &inventory = entity.addComponent<Inventory>();
        for (auto item_str: utils::split(args["items"], ","))
        {
            auto helper = utils::split(item_str, ":");
            auto item = services::ItemService::Instance().getItemByPrefab(helper[0]);
            if (item)
            {
                int amount = std::stoi(helper[1].c_str());
                inventory.addItem(item, SlotTarget::INVENTORY, amount);
            }
        }

        entity.addComponent<Character>();

        core::ecs::addScriptComponent<components::AnimalComponent>(entity);
    }

    void createApple(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
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
    void createChest(core::ecs::Entity &entity, utils::Vector2 &position, [[maybe_unused]] ArgsMap &args)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/chest.json");
        auto childTexture = itemTextureMap->getChildTexture("closed");
        core::ecs::Transform itemTransform;
        itemTransform.position = position;
        itemTransform.width = childTexture->getRect().width;
        itemTransform.height = childTexture->getRect().height;
        entity.addComponent<core::ecs::Transform>(itemTransform);
        auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
        rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Kinematic;

        auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
        collider.Offset = {0.5f, 0.5f};
        collider.Size = {1.0f, 1.0f};
        collider.Density = 0.5;

        entity.addComponent<core::ecs::RenderComponent>(childTexture);
        entity.addComponent<Inventory>();
        core::ecs::addScriptComponent<ChestComponent>(entity);

        auto &script = core::ecs::addScriptComponent<ItemEntity>(entity);
        auto item = services::ItemService::Instance().getItemByName("Chest");
        ((ItemEntity *)script.Instance)->setItem(item);
        ((ItemEntity *)script.Instance)->setTypeOfItemDestruction(TypeOfItemDestruction::RightClick);
    }
    void instantiateByItem(core::ecs::Entity &entity, utils::Vector2 &position, const std::shared_ptr<Item> &item)
    {
        auto &itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        auto childTexture = itemTextureMap->getChildTexture(item->getSubTextureName());
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
        ((ItemEntity *)script.Instance)->setItem(item);
    }

    // clang-format off
    static std::map<std::string, std::function<void(core::ecs::Entity &, utils::Vector2 &, ArgsMap &)>> prefabList = {
        {"campfire"s, createCampfire}, {"tree"s, createTree}, {"mountain"s, createMountain}, {"rock"s, createRock}
        ,{"wood"s, createWood}, {"stone"s, createStone}, {"apple"s, createApple}, {"coal"s, createCoal}
        ,{"iron_ore"s, createIronOre}, {"silver_ore"s, createSilverOre}, {"gold_ore"s, createGoldOre}, {"furnace", createFurnace}
        ,{"iron_bar"s, createIron}, {"silver_bar"s, createSilver}, {"gold_bar"s, createGold}
        , {"animal"s, createAnimal}, {"chest"s,createChest}
        };

    // clang-format on
    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position,
                               ArgsMap &args)
    {
        if (prefabList.count(prefabName))
        {
            prefabList.at(prefabName).operator()(entity, position, args);
        }
        else if (services::ItemService::Instance().hasItemForPrefab(prefabName))
        {
            auto item = services::ItemService::Instance().getItemByPrefab(prefabName);
            instantiateByItem(entity, position, item);
        }
        else
        {
            throw std::runtime_error("the prefab with the name " + prefabName + " does not exist!");
        }
    }


    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position)
    {
        ArgsMap args;
        instantiateFromPrefab(entity, prefabName, position, args);
    }


} // namespace prefabs
