#include "AnimalComponent.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include <random>
#include "Character.h"
#include "game/Inventory.h"
#include "game/prefabs/Prefab.h"

namespace components
{
    AnimalComponent::AnimalComponent(/* args */) {}

    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());


    void AnimalComponent::onClick(int button)
    {
        if (button == SDL_BUTTON_LEFT)
        {
            std::cout << "attack animal" << std::endl;
            auto &character = entity.findComponent<Character>();

            auto player = entity.getScene()->findEntityByName("player");
            auto &playerCharacter = player->findComponent<Character>();

            if (character.doAttack(playerCharacter))
            {
                if (entity.HasComponent<Inventory>())
                {
                    std::random_device device;
                    std::mt19937 gen(device());
                    std::uniform_real_distribution<double> posDist(-16.0, 16.0);

                    auto &inventory = entity.findComponent<Inventory>();
                    auto &transform = entity.findComponent<core::ecs::Transform>();
                    for (auto &slot: inventory.getItemSlots())
                    {
                        if (!slot.item)
                            continue;


                        std::uniform_int_distribution<size_t> amountDist(0, slot.amount + 1);

                        for (size_t i = 0; i < amountDist(gen); ++i)
                        {
                            auto position = transform.position +
                                            utils::Vector2{transform.width / 2.0f, float(transform.height)} +
                                            utils::Vector2(posDist(gen), posDist(gen));
                            auto itemEntity = entity.getScene()->createEntity(slot.item->getPrefab());
                            prefabs::instantiateFromPrefab(itemEntity, slot.item->getPrefab(), position);
                        }
                    }
                }

                entity.getScene()->destoryEntity(entity);
            }
        }
    }

    void AnimalComponent::onUpdate(size_t delta)
    {

        core::ecs::TextureMapAnimationRenderComponent &component =
                entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
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
            auto &rb2d = entity.findComponent<core::ecs::Rigidbody2DComponent>();
            if (component.animator.currentAnimation().isPlaying())
            {
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
            else
            {
                rb2d.SetLinearVelocity({.0f, .0f});
            }
        }
    }

} // namespace components
