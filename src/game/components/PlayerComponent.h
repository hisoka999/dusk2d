#pragma once
#include <engine/core/Timer.h>
#include <engine/core/ecs/Component.h>
#include <engine/utils/time/time.h>
#include "game/MovementDirection.h"
class PlayerComponent : public core::ecs::ScriptableEntity
{
private:
    MovementDirection direction;
    utils::time::Time gameTime;
    core::Timer timer;
    void setAnimation(const std::string &direction);

public:
    void onUpdate(size_t delta) override;
    void onCollision(ScriptableEntity *entity) override;
    bool onHandleInput(core::Input *input) override;

    PlayerComponent(/* args */);
    virtual ~PlayerComponent();
};
