#include "CraftingEntity.h"
#include <chrono>
#include <engine/core/ecs/Entity.h>
#include <engine/core/input.h>
#include "game/Inventory.h"
#include "game/services/ItemRecipeService.h"
#include "translate.h"
#include "ui/windows/CraftingWindow.h"
#include "ui/windows/PlayerWindow.h"

CraftingEntity::CraftingEntity() {}

void CraftingEntity::beginCollision([[maybe_unused]] const core::ecs::Collision &collider)
{
    if (collider.entity.compareTag("player"))
    {
        playerEntity = collider.entity;
    }
}

void CraftingEntity::endCollision([[maybe_unused]] const core::ecs::Collision &collider) { playerEntity = {}; }

void CraftingEntity::onClick(int button)
{
    if (button == SDL_BUTTON_LEFT)
    {
        auto craftingWindow = getEntity().getScene()->getWindowManager()->findContainer<UI::CraftingWindow>();
        auto playerWindow = getEntity().getScene()->getWindowManager()->findContainer<UI::PlayerWindow>();
        playerWindow->setVisible(true);
        craftingWindow->setVisible(true);
        craftingWindow->setEntity(this->getEntity());
        switch (recipeTarget)
        {
            case RecipeTarget::CAMPFIRE:
                craftingWindow->setTitle(_("Campfire"));
                break;
            case RecipeTarget::FURNACE:
                craftingWindow->setTitle(_("Furnace"));
                break;
            default:
                break;
        }

        playerWindow->setPos(craftingWindow->getX() + craftingWindow->getWidth() + 20, craftingWindow->getY());
    }
}

bool CraftingEntity::onHandleInput([[maybe_unused]] core::Input *input)
{
    if (input->isMouseButtonPressed(SDL_BUTTON_LEFT) && playerEntity)
    {
        onClick(SDL_BUTTON_LEFT);
    }

    return false;
}

void CraftingEntity::setRecipeTarget(RecipeTarget target) { recipeTarget = target; }

bool CraftingEntity::hasFuel()
{
    if (remainingFuel > 0)
        return true;
    auto &inventory = this->getEntity().findComponent<Inventory>();
    auto &slots = inventory.getHotBarSlots();
    for (size_t i = 4; i < slots.size(); ++i)
    {
        auto &itemSlot = slots.at(i);
        if (itemSlot.item && itemSlot.item->getItemSubType() == ItemSubType::FUEL && itemSlot.amount > 0)
        {
            return true;
        }
    }
    return false;
}

void CraftingEntity::onUpdate(size_t delta)
{
    if (!isCrafting)
        return;

    auto &inventory = this->getEntity().findComponent<Inventory>();
    // TODO now craft
    if (hasFuel())
    {
        if (remainingFuel == 0)
        {
            auto &slots = inventory.getHotBarSlots();

            for (size_t i = 4; i < slots.size(); ++i)
            {
                auto &itemSlot = slots.at(i);
                if (itemSlot.item && itemSlot.item->getItemSubType() == ItemSubType::FUEL && itemSlot.amount > 0)
                {
                    remainingFuel += std::atoi(itemSlot.item->getProperty(ItemProperty::fuel).c_str());
                    itemSlot.amount--;
                    if (itemSlot.amount == 0)
                        itemSlot.item = nullptr;
                    break;
                }
            }
            refuelTime = 20000;
        }

        refuelTime -= delta;
        if (refuelTime <= 0)
        {
            remainingFuel--;
            refuelTime = 20000;
        }
    }
    else
    {
        return;
    }
    // move to timer
    if (craftingQueue.empty())
    {
        bool added = false;
        for (auto recipe: services::ItemRecipeService::Instance().findByRecipeTarget(recipeTarget))
        {
            if (inventory.canCraftRecipe(recipe))
            {
                int64_t startTime = std::chrono::system_clock::now().time_since_epoch().count();
                int64_t endTime = startTime + (recipe->getCraftingTime() * 1000);
                craftingQueue.push_back(CraftingQueueEntry{recipe, startTime, startTime, endTime});
                added = true;
                break;
            }
        }
        if (!added)
        {
            isCrafting = false;
            CraftingQueueEntry empty{};
            m_progressCallback(empty, !isCrafting);
            auto &component = entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
            component.animator.currentAnimation().stop();
            component.animator.setCurrentAnimation("idle");
        }
    }
    else
    {
        auto &current = craftingQueue.front();

        current.currentTime += delta;
        if (current.currentTime >= current.endTime)
        {
            current.currentTime = current.endTime;
            inventory.craftItem(current.recipe, SlotTarget::INVENTORY);
            craftingQueue.erase(craftingQueue.begin());
        }
        m_progressCallback(current, !isCrafting);
    }
}

void CraftingEntity::progressCallBack(std::function<void(CraftingQueueEntry &, bool)> callback)
{
    m_progressCallback = callback;
}

void CraftingEntity::startCrafting()
{
    isCrafting = true;

    auto &component = entity.findComponent<core::ecs::TextureMapAnimationRenderComponent>();
    component.animator.currentAnimation().stop();
    component.animator.setCurrentAnimation("active");
    component.animator.currentAnimation().play();
}

CraftingEntity::~CraftingEntity() {}
