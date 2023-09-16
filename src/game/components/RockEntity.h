#pragma once

#include <engine/core/ecs/Component.h>

class RockEntity : public core::ecs::ScriptableEntity
{
private:
    core::ecs::Entity playerEntity;

public:
    RockEntity();
    ~RockEntity();
    bool onHandleInput(core::Input *input) override;
    virtual void beginCollision(const core::ecs::Collision &collider);
    virtual void endCollision(const core::ecs::Collision &collider);
};