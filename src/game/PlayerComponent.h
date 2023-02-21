#pragma once
#include <engine/core/ecs/Component.h>
#include "MovementDirection.h"

class PlayerComponent : public core::ecs::ScriptableEntity
{
private:
    MovementDirection direction;
    void setAnimation(const std::string &direction);

public:
    void onUpdate(size_t delta) override;
    void onCollision(ScriptableEntity *entity) override;
    bool onHandleInput(core::Input *input) override;

    PlayerComponent(/* args */);
    virtual ~PlayerComponent();
};
