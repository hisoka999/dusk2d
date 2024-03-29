#include "PlayerComponent.h"
#include <engine/core/ecs/Entity.h>
#include <engine/graphics/TextureManager.h>
#include "game/components/Character.h"
#include "game/messages.h"

void PlayerComponent::onUpdate(size_t delta)
{

    auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();
    auto &transform = entity.findComponent<core::ecs::Transform>();
    auto &character = entity.findComponent<Character>();
    character.updateAttributes(delta);

    auto camera = entity.getScene()->findEntityByName("mainCamera");
    auto &cameraComponent = camera->findComponent<core::ecs::CameraComponent>();
    //    transform.position += {1.0, 1.0};

    float moveX = 0.f;
    float moveY = 0.f;
    float speed = delta / 1000.f * 50.0f;
    // graphics::SpriteDirection spriteDirection = graphics::SpriteDirection::LEFT;
    MovementDirection collisionDirection;

    if (direction.left && !collisionDirection.left)
    {
        moveX -= speed;
    }
    else if (direction.right && !collisionDirection.right)
    {
        moveX += speed;
    }
    if (direction.top && !collisionDirection.top)
    {
        moveY -= speed;
    }
    else if (direction.bottom && !collisionDirection.bottom)
    {
        moveY += speed;
    }

    rb2d.SetLinearVelocity({moveX, moveY});

    cameraComponent.camera->reset();
    cameraComponent.camera->move(transform.position.getX() - (cameraComponent.camera->getWidth() / 2.0f),
                                 transform.position.getY() - (cameraComponent.camera->getHeight() / 2.0f));

    timer.update();
}

void PlayerComponent::onCollision(ScriptableEntity *entity)
{
    auto &transform = entity->getEntity().findComponent<core::ecs::Transform>();
    std::cout << "x: " << transform.position.getX() << " y: " << transform.position.getY() << std::endl;
}
void PlayerComponent::setAnimation(const std::string &direction)
{
    core::ecs::TextureMapAnimationRenderComponent &component =
            entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();

    if (component.animator.hasAnimation(direction))
    {
        component.animator.setCurrentAnimation(direction);
        return;
    }

    auto &textureMap = graphics::TextureManager::Instance().loadTextureMap("images/character.json");
    utils::Vector2 startPos = {0, 0};
    graphics::TextureMapAnimation animation(startPos, textureMap);
    animation.setRepeating(-1);
    for (int i = 1; i <= 9; i++)
    {
        std::string frame = direction + std::to_string(i);
        animation.createFrame<std::string>(startPos, 50, frame);
    }

    component.animator.addAnimation(direction, animation);
    component.animator.setCurrentAnimation(direction);
}
bool PlayerComponent::onHandleInput(core::Input *input)
{
    auto &animation = entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
    bool eventHandled = false;
    if (input->isKeyDown("MOVE_DOWN"))
    {
        direction.bottom = true;
        if (!animation.animator.currentAnimation().isPlaying())
        {
            setAnimation("down");
            animation.animator.currentAnimation().play();
        }
        eventHandled = true;
    }
    else if (input->isKeyDown("MOVE_UP"))
    {
        direction.top = true;
        if (!animation.animator.currentAnimation().isPlaying())
        {
            setAnimation("up");
            animation.animator.currentAnimation().play();
        }

        eventHandled = true;
    }

    if (input->isKeyDown("MOVE_LEFT"))
    {
        if (!animation.animator.currentAnimation().isPlaying())
        {
            setAnimation("left");
            animation.animator.currentAnimation().play();
        }
        direction.left = true;
        eventHandled = true;
    }
    else if (input->isKeyDown("MOVE_RIGHT"))
    {
        if (!animation.animator.currentAnimation().isPlaying())
        {
            setAnimation("right");
            animation.animator.currentAnimation().play();
        }
        direction.right = true;
        eventHandled = true;
    }
    if (input->isKeyUp("MOVE_RIGHT") || input->isKeyUp("MOVE_LEFT") || input->isKeyUp("MOVE_UP") ||
        input->isKeyUp("MOVE_DOWN"))
    {
        eventHandled = true;
        animation.animator.currentAnimation().stop();
        direction.right = false;
        direction.left = false;
        direction.top = false;
        direction.bottom = false;
    }
    return eventHandled;
}

PlayerComponent::PlayerComponent() : gameTime(2050, 2, 5, 22, 10, 0), timer(250, -1)
{
    timer.setCallback(
            [this]([[maybe_unused]] uint32_t execs)
            {
                this->gameTime.addMinutes(1);
                core::MessageSystem<MessageType>::get().sendMessage(MessageType::TIME_CHANGED, this->gameTime);
            });
    timer.start();
}

PlayerComponent::~PlayerComponent() {}
