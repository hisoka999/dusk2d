#include "PlayerComponent.h"
#include <engine/core/ecs/Entity.h>
#include <engine/graphics/TextureManager.h>
void PlayerComponent::onUpdate(size_t delta)
{

    auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();
    auto &transform = entity.findComponent<core::ecs::Transform>();

    auto camera = entity.getScene()->findEntityByName("mainCamera");
    auto &cameraComponent = camera->findComponent<core::ecs::CameraComponent>();
    //    transform.position += {1.0, 1.0};

    float moveX = 0.f;
    float moveY = 0.f;
    float speed = delta / 1000.f * 40.25f;
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
    // rb2d.applyLinearImpuls({moveX, moveY}, true);
    b2Body *body = (b2Body *)rb2d.RuntimeBody;
    body->SetLinearVelocity({moveX, moveY});

    // body->SetTransform(b2Vec2(body->GetTransform().p.x + moveX, body->GetTransform().p.y + moveY), 0);

    cameraComponent.camera->reset();
    cameraComponent.camera->move(transform.position.getX() - (cameraComponent.camera->getWidth() / 2.0f), transform.position.getY() - (cameraComponent.camera->getHeight() / 2.0f));
    //  transform.position = utils::Vector2{cameraComponent.camera->getX() + (cameraComponent.camera->getWidth() / 2), cameraComponent.camera->getY() + (cameraComponent.camera->getHeight() / 2)}; //+= utils::Vector2{moveX, moveY};
    //  std::cout << "x: " << collider.body.position.getX() << " y: " << collider.body.position.getY() << std::endl;
}

void PlayerComponent::onCollision(ScriptableEntity *entity)
{
    auto &transform = entity->getEntity().findComponent<core::ecs::Transform>();
    std::cout << "x: " << transform.position.getX() << " y: " << transform.position.getY() << std::endl;
}
void PlayerComponent::setAnimation(const std::string &direction)
{
    auto textureMap = graphics::TextureManager::Instance().loadTextureMap("images/character.json");
    utils::Vector2 startPos = {0, 0};
    graphics::TextureMapAnimation animation(startPos, textureMap);
    animation.setRepeating(-1);
    for (int i = 1; i <= 9; i++)
    {
        std::string frame = direction + std::to_string(i);
        animation.createFrame<std::string>(startPos, 50, frame);
    }

    core::ecs::TextureMapAnimationRenderComponent &component = entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
    component.animation = animation;
}
bool PlayerComponent::onHandleInput(core::Input *input)
{
    auto &animation = entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
    bool eventHandled = false;
    if (input->isKeyDown("MOVE_DOWN"))
    {
        direction.bottom = true;
        if (!animation.animation.isPlaying())
        {
            setAnimation("down");
            animation.animation.play();
        }
        eventHandled = true;
    }
    else if (input->isKeyDown("MOVE_UP"))
    {
        direction.top = true;
        if (!animation.animation.isPlaying())
        {
            setAnimation("up");
            animation.animation.play();
        }

        eventHandled = true;
    }
    else
    {
        eventHandled = true;
    }

    if (input->isKeyDown("MOVE_LEFT"))
    {
        if (!animation.animation.isPlaying())
        {
            setAnimation("left");
            animation.animation.play();
        }
        direction.left = true;
        eventHandled = true;
    }
    else if (input->isKeyDown("MOVE_RIGHT"))
    {
        if (!animation.animation.isPlaying())
        {
            setAnimation("right");
            animation.animation.play();
        }
        direction.right = true;
        eventHandled = true;
    }
    if (input->isKeyUp("MOVE_RIGHT") || input->isKeyUp("MOVE_LEFT") || input->isKeyUp("MOVE_UP") || input->isKeyUp("MOVE_DOWN"))
    {
        eventHandled = true;
        animation.animation.stop();
        direction.right = false;
        direction.left = false;
        direction.top = false;
        direction.bottom = false;
    }
    return eventHandled;
}

PlayerComponent::PlayerComponent()
{
}

PlayerComponent::~PlayerComponent()
{
}
