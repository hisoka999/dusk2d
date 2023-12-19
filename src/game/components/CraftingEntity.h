#pragma once

#include <engine/core/ecs/Component.h>
#include "game/ItemRecipe.h"
#include <vector>
#include <functional>

struct CraftingQueueEntry
{
    std::shared_ptr<ItemRecipe> recipe;
    int64_t startTime;
    int64_t currentTime;
    int64_t endTime;
};

class CraftingEntity : public core::ecs::ScriptableEntity
{
public:
    CraftingEntity();
    ~CraftingEntity();
    bool onHandleInput(core::Input *input) override;
    void beginCollision(const core::ecs::Collision &collider) override;
    void endCollision(const core::ecs::Collision &collider) override;
    void onUpdate(size_t delta) override;
    void startCrafting();
    void progressCallBack(std::function<void(CraftingQueueEntry &, bool)> callback);

private:
    bool hasFuel();

    core::ecs::Entity playerEntity;
    std::vector<CraftingQueueEntry> craftingQueue;
    bool isCrafting = false;
    std::function<void(CraftingQueueEntry &, bool)> m_progressCallback;
    int32_t remainingFuel = 0;
    int64_t refuelTime = 0;
};
