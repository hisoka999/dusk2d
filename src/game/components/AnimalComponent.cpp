#include "AnimalComponent.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <random>

namespace components
{
    AnimalComponent::AnimalComponent(/* args */) {}

    void AnimalComponent::onClick(int button)
    {
        if (button == SDL_BUTTON_LEFT)
        {
        }
    }

    void AnimalComponent::onUpdate(size_t delta)
    {

        core::ecs::TextureMapAnimationRenderComponent &component =
                entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
        std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(0, 8);

        std::uniform_int_distribution<int> walkTimeDist(500, 3000);

        if (walkTime <= 0)
        {
            direction = {};
            switch (distribution(gen))
            {
                case 0:
                    component.animator.setCurrentAnimation("left");
                    component.animator.currentAnimation().play();
                    direction.left = true;

                    break;
                case 1:
                    component.animator.setCurrentAnimation("right");
                    component.animator.currentAnimation().play();
                    direction.right = true;
                    break;
                case 2:
                    component.animator.setCurrentAnimation("up");
                    component.animator.currentAnimation().play();
                    direction.top = true;
                    break;
                case 3:
                    component.animator.setCurrentAnimation("down");
                    component.animator.currentAnimation().play();
                    direction.bottom = true;
                    break;
                default:
                    component.animator.currentAnimation().stop();
                    break;
            }
            walkTime = walkTimeDist(gen);
        }
        else
        {
            walkTime -= delta;
            if (component.animator.currentAnimation().isPlaying())
            {
                auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();

                float moveX = 0.f;
                float moveY = 0.f;
                float speed = delta / 1000.f * 50.0f;
                if (direction.left)
                {
                    moveX -= speed;
                }
                else if (direction.right)
                {
                    moveX += speed;
                }
                if (direction.top)
                {
                    moveY -= speed;
                }
                else if (direction.bottom)
                {
                    moveY += speed;
                }

                rb2d.SetLinearVelocity({moveX, moveY});
            }
        }
    }

} // namespace components
