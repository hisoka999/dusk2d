#include "InventoryComponent.h"
#include <ui/InventorySlot.h>
#include "game/Inventory.h"

namespace UI
{
    InventoryComponent::InventoryComponent(UI::Object *parent, const core::ecs::Entity &entity) : UI::Object(parent), entity(entity)
    {
    }

    InventoryComponent::~InventoryComponent()
    {
    }

    void InventoryComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    void InventoryComponent::postRender(core::Renderer *pRender)
    {
        UI::Container::postRender(pRender);
    }

    bool InventoryComponent::handleEvents(core::Input *pInput)
    {
        return UI::Container::handleEvents(pInput);
    }

    void InventoryComponent::refresh()
    {
        clear();
        auto &inventory = entity.findComponent<Inventory>();
        auto &slots = inventory.getItemSlots();
        size_t i = 0;
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 5; x++)
            {
                auto uiSlot = std::make_shared<UI::InventorySlot>(this, slots[i]);
                uiSlot->setPos(x * uiSlot->getWidth(), y * uiSlot->getHeight());
                addObject(uiSlot);

                i++;
            }
        }
    }
}