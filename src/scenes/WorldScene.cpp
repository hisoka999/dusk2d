#include "WorldScene.h"
#include <chrono>
#include <engine/core/RayCastResult.h>
#include <engine/core/ecs/Component.h>
#include <engine/core/ecs/ScriptComponent.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/string.h>
#include <random>
#include "game/Inventory.h"
#include "game/RockType.h"
#include "game/Sprite.h"
#include "game/components/Character.h"
#include "game/components/PlayerComponent.h"
#include "game/messages.h"
#include "game/prefabs/Prefab.h"
namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer) :
        core::Scene(pRenderer), gameMap(200, 200, std::chrono::system_clock::now().time_since_epoch().count())
    {

        setPixelPerMeter(32.f);
        itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");
        mountainTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/mountain.json");
        coalTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/coal.json");
        ironTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/iron.json");
        silverTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/silver.json");
        goldTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/gold.json");
        std::vector<std::shared_ptr<graphics::TextureMap>> textureMaps = {
                mountainTextureMap, coalTextureMap, ironTextureMap, silverTextureMap, goldTextureMap};
        mountainLayer = std::make_unique<AutotileLayer>(gameMap.getWidth(), gameMap.getHeight(), textureMaps);

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> entityDistribution(0, 1000);
        std::uniform_int_distribution<int> rockDistribution(1, 100);

        std::uniform_int_distribution<int> xPositionDistribution(0, gameMap.getWidth());
        std::uniform_int_distribution<int> yPositionDistribution(0, gameMap.getHeight());

        {
            auto entity = createEntity("mainCamera");
            entity.addComponent<core::ecs::CameraComponent>(renderer->getMainCamera());
        }

        for (size_t y = 0; y < gameMap.getHeight(); ++y)
        {
            for (size_t x = 0; x < gameMap.getWidth(); ++x)
            {
                TileType tile = gameMap.getTile(x, y);
                if (tile == 2)
                {
                    int value = entityDistribution(gen);
                    std::string type;
                    prefabs::ArgsMap args;
                    if (value == 73)
                    {
                        type = "animal";
                        args["offset_x"] = "0.5";
                        args["offset_y"] = "0.5";
                        args["name"] = "chicken";
                        args["items"] = "drumstick:2,egg:2";
                    }
                    else if (value == 72)
                    {
                        type = "animal";
                        args["offset_x"] = "2.0";
                        args["offset_y"] = "2.0";
                        args["name"] = "sheep";
                        args["items"] = "whool:3,sheep_steak:2";
                    }
                    else if (value == 71)
                    {
                        type = "animal";
                        args["offset_x"] = "2.0";
                        args["offset_y"] = "2.0";
                        args["name"] = "cow";
                        args["items"] = "raw_steak:5,leather:3";
                    }
                    else if (value > 50 && value <= 70)
                    {
                        type = "rock";
                    }
                    else if (value > 70 && value <= 100)
                    {
                        type = "wood";
                    }
                    else if (value <= 30)
                    {
                        type = "tree";
                    }

                    if (!type.empty())
                    {
                        auto entity = createEntity(type + std::to_string(x) + "_" + "" + std::to_string(y));
                        auto pos = utils::Vector2{static_cast<float>(x * TILE_SIZE / 2),
                                                  static_cast<float>(y * TILE_SIZE / 2)};
                        prefabs::instantiateFromPrefab(entity, type, pos, args);
                    }
                }
                else if (tile == 1)
                {
                    int value = entityDistribution(gen);
                    std::string type = "";
                    if (value > 50 && value <= 70)
                    {
                        type = "rock";
                    }
                    else if (value > 70 && value <= 100)
                    {
                        type = "wood";
                    }

                    if (!type.empty())
                    {
                        auto entity = createEntity(type + std::to_string(x) + "_" + "" + std::to_string(y));
                        auto pos = utils::Vector2{static_cast<float>(x * TILE_SIZE / 2),
                                                  static_cast<float>(y * TILE_SIZE / 2)};
                        prefabs::instantiateFromPrefab(entity, type, pos);
                    }
                }
                else if (tile == 3)
                {
                    auto entity = createEntity("mountain_" + std::to_string(x) + "_" + "" + std::to_string(y));
                    auto pos = utils::Vector2{static_cast<float>(x * TILE_SIZE / 2),
                                              static_cast<float>(y * TILE_SIZE / 2)};
                    // prefabs::instantiateFromPrefab(entity, "mountain", pos);
                    auto rockPercentage = rockDistribution(gen);
                    if (rockPercentage < 85)
                    {
                        mountainLayer->setTile(x, y, 1);
                    }
                    else if (rockPercentage < 90)
                    {
                        mountainLayer->setTile(x, y, 2);
                    }
                    else if (rockPercentage < 95)
                    {
                        mountainLayer->setTile(x, y, 3);
                    }
                    else if (rockPercentage < 97)
                    {
                        mountainLayer->setTile(x, y, 4);
                    }
                    else if (rockPercentage < 100)
                    {
                        mountainLayer->setTile(x, y, 5);
                    }
                }
            }
        }

        auto playerSprite = std::make_shared<Sprite>("images/walkcycle/BODY_male.png", 4, 9);
        renderer->setZoomFactor(1);
        core::ecs::Transform playerTransform;
        {
            auto entity = createEntity("player");

            bool positionInvalid = true;
            do
            {
                playerSprite->setPosition(xPositionDistribution(gen) * (TILE_SIZE / 2),
                                          yPositionDistribution(gen) * (TILE_SIZE / 2));
                playerTransform.position = playerSprite->getRect().toVector2();

                auto tile = gameMap.getTile(playerTransform.position.getX() / (TILE_SIZE / 2),
                                            playerTransform.position.getY() / (TILE_SIZE / 2));
                positionInvalid = tile == 0 || tile == 3;
            }
            while (positionInvalid);

            playerTransform.width = playerSprite->getRect().width;
            playerTransform.height = playerSprite->getRect().height;
            playerTransform = entity.addComponent<core::ecs::Transform>(playerTransform);
            auto textureMap = graphics::TextureManager::Instance().loadTextureMap("images/character.json");
            utils::Vector2 startPos = {0, 0};
            graphics::TextureMapAnimator animator;
            graphics::TextureMapAnimation animation(startPos, textureMap);
            animation.setRepeating(-1);
            animation.createFrame<std::string>(startPos, 50, std::string{"left1"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left2"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left3"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left4"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left5"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left6"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left7"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left8"});
            animation.createFrame<std::string>(startPos, 50, std::string{"left9"});
            animator.addAnimation("left", animation);
            animator.setCurrentAnimation("left");
            entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animator);
            core::ecs::addScriptComponent<PlayerComponent>(entity);
            auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
            auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
            entity.addComponent<Character>();
            rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Dynamic;
            rb2d.FixedRotation = true;
            collider.Density = 1.f;
            collider.Friction = 0;
            collider.Restitution = 0.0;
            collider.RestitutionThreshold = 0.0;
            collider.Offset = {1.f, 1.5f};
            collider.Size = {1.f, 1.f};

            entity.addComponent<Inventory>();

            playerWindow = std::make_shared<UI::PlayerWindow>(entity);
            winMgr->addWindow(playerWindow.get());
            hotBar = std::make_unique<UI::Hotbar>(entity);
            winMgr->addContainer(hotBar.get());

            playerHUD = std::make_unique<UI::PlayerHUD>();
            winMgr->addContainer(playerHUD.get());
        }

        craftingWindow = std::make_unique<UI::CraftingWindow>(10, 200, "Crafting Window");
        winMgr->addWindow(craftingWindow.get());

        chestWindow = std::make_unique<UI::ChestWindow>(10, 200);
        winMgr->addWindow(chestWindow.get());

        addStaticBlockCollider(gameMap.generateCollisionMap());
        addStaticBlockCollider(mountainLayer->generateCollisionMap());

        OnPhysics2DStart();
    }

    WorldScene::~WorldScene() {}

    void WorldScene::render()
    {
        gameMap.render(renderer);
        mountainLayer->render(renderer);
        renderEntities(renderer);
        winMgr->render(renderer);

        auto &slot = this->hotBar->selectedSlot();
        if (slot.item != nullptr && slot.item->getType() == ItemType::PLACEABLE)
        {
            // render preview
            auto childTexture = itemTextureMap->getChildTexture(slot.item->getSubTextureName());
            graphics::Rect destRect{mousePos.getX(), mousePos.getY(), childTexture->getRect().width,
                                    childTexture->getRect().height};
            childTexture->render(destRect, renderer);
        }

        // render mouse pos
        if (!winMgr->isWindowOpen())
        {
            const int tileSize = TILE_SIZE / 2;
            const auto pos =
                    (getMouseMapPos() * utils::Vector2(tileSize, tileSize)) - renderer->getMainCamera()->getPosition();

            graphics::Rect r{pos.getX(), pos.getY(), tileSize, tileSize};

            renderer->drawRect(r);
        }
    }

    utils::Vector2 WorldScene::getMouseMapPos() const
    {
        constexpr int tileSize = TILE_SIZE / 2;
        const auto camera = renderer->getMainCamera()->getPosition();
        const auto cameraOffset =
                utils::Vector2{(std::round(camera.getX() / tileSize)), (std::round(camera.getY() / tileSize))};
        return {std::round(mousePos.getX() / tileSize) + cameraOffset.getX(),
                std::round(mousePos.getY() / tileSize) + cameraOffset.getY()};
    }

    TileType WorldScene::getTileOnMouse()
    {
        auto pos = getMouseMapPos();
        return gameMap.getTile(static_cast<size_t>(pos.getX()), static_cast<size_t>(pos.getY()));
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        mousePos = pInput->getMousePostion();
        bool handled = winMgr->handleInput(pInput);
        if (!handled)
            handled = handleEventsEntities(pInput);

        if (pInput->isKeyDown(SDLK_i))
        {
            playerWindow->setVisible(true);
        }
        else if (pInput->isKeyDown(SDLK_TAB))
        {
            setPhysicsDebug(!getPhysicsDebug());
        }

        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            APP_LOG_INFO("tile: " + std::to_string(static_cast<int>(getTileOnMouse())));
            const int tileSize = TILE_SIZE / 2;
            auto pos = getMouseMapPos() * utils::Vector2(tileSize, tileSize);
            auto endPos = pos + (tileSize);
            APP_LOG_ERROR("raycast start %.2f,%.2f", pos.getX(), pos.getY());
            APP_LOG_ERROR("raycast end %.2f,%.2f", endPos.getX(), endPos.getY());

            auto result = raycast(pos - 1, endPos);
            auto playerEntity = findEntityByName("player");
            auto &transform = playerEntity->findComponent<core::ecs::Transform>();
            auto &character = playerEntity->findComponent<Character>();
            auto &inventory = playerEntity->findComponent<Inventory>();
            auto &selectedHotbarSlot = inventory.getSelectedHotbarSlot();

            for (auto &hit: result.hits)
            {
                if (hit.getEntity().has_value())
                {
                    APP_LOG_INFO("Entity: " + hit.getEntity()->tagName());

                    auto &list = hit.getEntity()->findComponent<core::ecs::ScriptComponentList>();
                    for (auto &script: list.components)
                    {
                        script.Instance->onClick(SDL_BUTTON_LEFT);
                    }
                }
                else
                {
                    auto data = utils::split(hit.getCollisionBlock()->blockData, ":");
                    APP_LOG_INFO("CollisionBlock: " + hit.getCollisionBlock()->blockData);
                    APP_LOG_INFO("CollisionBlock ID: " + std::to_string(hit.getCollisionBlock()->blockId));

                    if (data[0] == "tile" && data[1] == "0")
                    {

                        if (transform.position.distance(hit.getPosition()) < 200)
                        {
                            character.getThirst().addValue(20);
                        }
                    }
                    else if (data[0] == "mountain" && data[1] != "0" && selectedHotbarSlot.item &&
                             selectedHotbarSlot.item->getItemSubType() == ItemSubType::PICK_AXE)
                    {
                        size_t x = std::strtol(data[2].c_str(), nullptr, 10);
                        size_t y = std::strtol(data[3].c_str(), nullptr, 10);
                        mountainLayer->setTile(x, y, 0);
                        auto rockType = static_cast<RockType>(std::strtol(data[1].c_str(), nullptr, 10));

                        std::random_device device;
                        std::mt19937 gen(device());
                        std::uniform_real_distribution<float> posDist(0.0, 16.0);
                        std::uniform_int_distribution<int> numItems(2, 5);
                        std::string entityName = "stone";
                        switch (rockType)
                        {
                            case RockType::Rock:
                                break;
                            case RockType::Iron:
                                entityName = "iron_ore";
                                break;
                            case RockType::Coal:
                                entityName = "coal";
                                break;
                            case RockType::Gold:
                                entityName = "gold_ore";
                                break;
                            case RockType::Silver:
                                entityName = "silver_ore";
                                break;
                            default:
                                break;
                        }

                        for (int i = 1; i <= numItems(gen); ++i)
                        {
                            auto position = hit.getPosition() + utils::Vector2(posDist(gen), posDist(gen));
                            auto itemEntity = createEntity(entityName);

                            prefabs::instantiateFromPrefab(itemEntity, entityName, position);
                        }
                        removeStaticBlockCollider(hit.getCollisionBlock()->blockData);
                    }
                }
            }
        }

        if (!handled && !pInput->isDragActive() && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            auto &slot = this->hotBar->selectedSlot();

            if (slot.item != nullptr && slot.item->getType() == ItemType::PLACEABLE)
            {
                // TODO spawn item in world
                auto entity = createEntity(slot.item->getName());
                auto playerEntity = findEntityByName("player");
                auto &transform = playerEntity->findComponent<core::ecs::Transform>();
                const int tileSize = TILE_SIZE / 2;

                auto pos = getMouseMapPos() * utils::Vector2(tileSize, tileSize);

                APP_LOG_ERROR("position for new entity %.2f,%.2f", transform.position.getX(),
                              transform.position.getY());
                prefabs::instantiateFromPrefab(entity, slot.item->getPrefab(), pos);
                auto &inventory = playerEntity->findComponent<Inventory>();
                inventory.removeItemById(slot.item->getId(), slot.amount);
            }
        }
        else if (!handled && pInput->isMouseButtonUp(SDL_BUTTON_RIGHT))
        {
        }

        return handled;
    }

    void WorldScene::fixedUpdate(uint32_t delta)
    {
        fixedUpdateEntities(delta);
        core::MessageSystem<MessageType>::get().processMessages();
    }

    void WorldScene::load() {}
} // namespace scenes
