#include "Prefab.h"
#include <engine/core/ecs/Entity.h>
#include <engine/graphics/TextureManager.h>
#include <engine/core/ecs/Component.h>
#include <map>
#include "game/CampfireEntity.h"
#include "game/TreeEntity.h"

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
        auto &script = entity.addComponent<core::ecs::ScriptComponent>();
        script.Bind<CampfireEntity>();
        script.Instance = script.InstantiateScript();
        script.Instance->setEntity(entity);
    }

    void createTree(core::ecs::Entity &entity, utils::Vector2 &position)
    {
        auto &treeTexture = graphics::TextureManager::Instance().loadTexture("images/trees/pine_tree.png");

        core::ecs::Transform transform;
        transform.position = position;
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

    static std::map<std::string, std::function<void(core::ecs::Entity &, utils::Vector2 &)>> prefabList = {{"campfire"s, createCampfire}, {"tree"s, createTree}};

    void instantiateFromPrefab(core::ecs::Entity &entity, const std::string &prefabName, utils::Vector2 &position)
    {
        if (prefabList.count(prefabName))
        {
            prefabList.at(prefabName).operator()(entity, position);
        }
    }
} // namespace prefabs
