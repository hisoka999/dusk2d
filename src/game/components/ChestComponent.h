#pragma once
#include <engine/core/ecs/Component.h>

class ChestComponent : public core::ecs::ScriptableEntity
{
private:
    /* data */
public:
    void onClick(int button) override;

    ChestComponent() = default;
    ~ChestComponent() = default;
};
