#pragma once
#include <engine/core/ecs/Component.h>
#include "game/MovementDirection.h"

namespace components
{
    class AnimalComponent : public core::ecs::ScriptableEntity
    {
    private:
        int32_t walkTime = 0;
        MovementDirection direction;

    public:
        AnimalComponent(/* args */);
        ~AnimalComponent() = default;
        void onUpdate(size_t delta) override;
        void onClick(int button) override;
    };


} // namespace components
