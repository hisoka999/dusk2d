#include "WorldScene.h"
#include <engine/graphics/TextureManager.h>
#include <engine/core/ecs/Component.h>
#include "game/Sprite.h"
#include "game/PlayerComponent.h"
#include "game/TreeEntity.h"
#include "game/RockEntity.h"
#include <chrono>
#include <random>
#include "game/Inventory.h"
#include "game/messages.h"

namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer)
        : core::Scene(pRenderer), gameMap(100, 100)
    {

        setPixelPerMeter(32.f);
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
            auto &script = entity.addComponent<core::ecs::ScriptComponent>();
            auto &collider = entity.addComponent<core::ecs::BoxCollider2DComponent>();
            auto &rb2d = entity.addComponent<core::ecs::Rigidbody2DComponent>();
            rb2d.Type = core::ecs::Rigidbody2DComponent::BodyType::Dynamic;
            rb2d.FixedRotation = true;
            collider.Density = 1.f;
            collider.Friction = 0;
            collider.Restitution = 0.0;
            collider.RestitutionThreshold = 0.0;
            collider.Offset = {1.f, 1.5f};
            collider.Size = {1.f, 1.f};
            script.Bind<PlayerComponent>();
            script.Instance = script.InstantiateScript();
            script.Instance->setEntity(entity);
            entity.addComponent<Inventory>();

            playerWindow = std::make_shared<UI::PlayerWindow>(entity);
            winMgr->addWindow(playerWindow.get());
            hotBar = std::make_unique<UI::Hotbar>(entity);
            winMgr->addContainer(hotBar.get());
        }

        auto treeTexture = graphics::TextureManager::Instance().loadTexture("images/trees/pine_tree.png");
        auto rockTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/rock.json");
        for (size_t y = 0; y < gameMap.getHeight(); ++y)
        {
            for (size_t x = 0; x < gameMap.getWidth(); ++x)
            {
                TileType tile = gameMap.getTile(x, y);
                int value = dist(gen);
                if (tile == 2 && value <= 1)
                {
                    std::cout << "random value: " << value << std::endl;

                    auto entity = createEntity("tree_" + std::to_string(x) + "_" + "" + std::to_string(y));
                    core::ecs::Transform transform;
                    transform.position = {float(x * TILE_SIZE / 2), float(y * TILE_SIZE / 2)};
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

                    auto &script = entity.addComponent<core::ecs::ScriptComponent>();
                    script.Bind<TreeEntity>();
                    script.Instance = script.InstantiateScript();
                    script.Instance->setEntity(entity);
                }
                else if (tile == 3)
                {
                    auto entity = createEntity("rock_" + std::to_string(x) + "_" + "" + std::to_string(y));
                    core::ecs::Transform transform;
                    transform.position = {float(x * TILE_SIZE / 2), float(y * TILE_SIZE / 2)};
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

                    auto &script = entity.addComponent<core::ecs::ScriptComponent>();
                    script.Bind<RockEntity>();
                    script.Instance = script.InstantiateScript();
                    script.Instance->setEntity(entity);
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
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
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