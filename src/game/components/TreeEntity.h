#pragma once

#include <engine/core/ecs/Component.h>

class TreeEntity : public core::ecs::ScriptableEntity
{
private:
    core::ecs::Entity playerEntity;

public:
    TreeEntity();
    ~TreeEntity();
    bool onHandleInput(core::Input *input) override;
    virtual void beginCollision(const core::ecs::Collision &collider) override;
    virtual void endCollision(const core::ecs::Collision &collider) override;
    void onClick(int button) override;
};
