#include "WorldScene.h"
#include <engine/graphics/TextureManager.h>
#include <engine/core/ecs/Component.h>
#include "game/Sprite.h"
#include "game/components/PlayerComponent.h"
#include "game/components/TreeEntity.h"
#include "game/components/RockEntity.h"
#include <chrono>
#include <random>
#include "game/Inventory.h"
#include "game/messages.h"
#include "game/prefabs/Prefab.h"
#include <engine/core/ecs/ScriptComponent.h>
#include "game/components/Character.h"

namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer)
        : core::Scene(pRenderer), gameMap(100, 100, std::chrono::system_clock::now().time_since_epoch().count())
    {

        setPixelPerMeter(32.f);
        itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(0, 100);

        {
            auto entity = createEntity("mainCamera");
            entity.addComponent<core::ecs::CameraComponent>(renderer->getMainCamera());
        }
        auto playerSprite = std::make_shared<Sprite>("images/walkcycle/BODY_male.png", 4, 9);
        playerSprite->setPosition(renderer->getMainCamera()->getWidth() / 2.0f, renderer->getMainCamera()->getHeight() / 2.0f);
        renderer->setZoomFactor(1);
        {
            auto entity = createEntity("player");
            core::ecs::Transform transform;
            transform.position = {playerSprite->getRect().x, playerSprite->getRect().y};
            transform.width = playerSprite->getRect().width;
            transform.height = playerSprite->getRect().height;
            entity.addComponent<core::ecs::Transform>(transform);
            auto textureMap = graphics::TextureManager::Instance().loadTextureMap("images/character.json");
            utils::Vector2 startPos = {0, 0};
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
            entity.addComponent<core::ecs::TextureMapAnimationRenderComponent>(animation);
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

        for (size_t y = 0; y < gameMap.getHeight(); ++y)
        {
            for (size_t x = 0; x < gameMap.getWidth(); ++x)
            {
                TileType tile = gameMap.getTile(x, y);
                if (tile == 2)
                {
                    int value = dist(gen);
                    std::string type = "";
                    if (value > 5 && value <= 7)
                    {
                        type = "rock";
                    }
                    else if (value > 7 && value <= 10)
                    {
                        type = "wood";
                    }
                    else if (value <= 3)
                    {
                        type = "tree";
                    }

                    if (!type.empty())
                    {
                        auto entity = createEntity(type + std::to_string(x) + "_" + "" + std::to_string(y));
                        auto pos = utils::Vector2{float(x * TILE_SIZE / 2), float(y * TILE_SIZE / 2)};
                        prefabs::instantiateFromPrefab(entity, type, pos);
                    }
                }
                else if (tile == 3)
                {
                    auto entity = createEntity("mountain_" + std::to_string(x) + "_" + "" + std::to_string(y));
                    auto pos = utils::Vector2{float(x * TILE_SIZE / 2), float(y * TILE_SIZE / 2)};
                    prefabs::instantiateFromPrefab(entity, "mountain", pos);
                }
            }
        }

        OnPhysics2DStart();
    }

    WorldScene::~WorldScene()
    {
    }

    void WorldScene::render()
    {
        gameMap.render(renderer);
        renderEntities(renderer);
        winMgr->render(renderer);

        auto &slot = this->hotBar->selectedSlot();
        if (slot.item != nullptr && slot.item->getType() == ItemType::PLACEABLE)
        {
            // render preview
            auto childTexture = itemTextureMap->getChildTexture(slot.item->getSubTextureName());
            graphics::Rect destRect{mousePos.getX(), mousePos.getY(), childTexture->getRect().width, childTexture->getRect().height};
            childTexture->render(destRect, renderer);
        }
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

        if (!handled && !pInput->isDragActive() && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            auto &slot = this->hotBar->selectedSlot();

            if (slot.item != nullptr && slot.item->getType() == ItemType::PLACEABLE)
            {
                // TODO spawn item in world
                auto entity = createEntity(slot.item->getName());
                auto playerEntity = findEntityByName("player");
                auto &transform = playerEntity->findComponent<core::ecs::Transform>();
                APP_LOG_ERROR("position for new entity %.2f,%.2f", transform.position.getX(), transform.position.getY());
                prefabs::instantiateFromPrefab(entity, slot.item->getPrefab(), transform.position);
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

    void WorldScene::load()
    {
    }
}