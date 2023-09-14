#include "Hotbar.h"
#include "game/Inventory.h"
#include "ui/InventorySlot.h"
#include "game/messages.h"

namespace UI
{
    Hotbar::Hotbar(const core::ecs::Entity &entity) : entity(entity)
    {
        needsRefresh();
        auto &msgSystem = core::MessageSystem<MessageType>::get();
        inventoryRefreshMsgId = msgSystem.registerForType(MessageType::INVENTORY_UPDATED, [this]([[maybe_unused]] ItemSlot *slot)
                                                          { this->needsRefresh(); });
    }

    void Hotbar::refresh()
    {
        this->clear();
        const auto &inventory = entity.findComponent<Inventory>();

        for (auto &itemSlot : inventory.getHotBarSlots())
        {
            auto slot = std::make_shared<UI::InventorySlot>(nullptr, itemSlot, entity);

            addObject(slot);
        }
        std::dynamic_pointer_cast<UI::InventorySlot>(objects[0])->setSelected(true);
        initSize = false;
        endRefresh();
    }

    Hotbar::~Hotbar()
    {
        auto &msgSystem = core::MessageSystem<MessageType>::get();
        msgSystem.deregister(inventoryRefreshMsgId);
    }
    void Hotbar::render(core::Renderer *renderer)
    {
        if (!initSize)
        {
            int yOffset = renderer->getMainCamera()->getHeight() - 60;
            int xOffset = (renderer->getMainCamera()->getWidth() / 2) - 100;

            for (auto &object : objects)
            {
                object->setPos(xOffset, yOffset);
                xOffset += object->getWidth();
            }
            initSize = true;
        }

        UI::Container::render(renderer);
    }

    bool Hotbar::handleEvents(core::Input *pInput)
    {
        if (pInput->isKeyDown(SDLK_1))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[0])->setSelected(true);
        }
        else if (pInput->isKeyDown(SDLK_2))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[1])->setSelected(true);
        }
        else if (pInput->isKeyDown(SDLK_3))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[2])->setSelected(true);
        }
        else if (pInput->isKeyDown(SDLK_4))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[3])->setSelected(true);
        }
        else if (pInput->isKeyDown(SDLK_5))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[4])->setSelected(true);
        }
        else if (pInput->isKeyDown(SDLK_6))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[5])->setSelected(true);
        }
        return UI::Container::handleEvents(pInput);
    }

    void Hotbar::clearSelection()
    {
        for (auto &object : objects)
        {
            std::dynamic_pointer_cast<UI::InventorySlot>(object)->setSelected(false);
        }
    }

    ItemSlot &Hotbar::selectedSlot()
    {
        for (auto &object : objects)
        {
            auto slot = std::dynamic_pointer_cast<UI::InventorySlot>(object);
            if (slot->isSelected())
            {
                return slot->getSlot();
            }
        }
        return std::dynamic_pointer_cast<UI::InventorySlot>(objects[0])->getSlot();
    }
} // namespace UI
