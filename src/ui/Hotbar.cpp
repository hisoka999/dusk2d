#include "Hotbar.h"
#include "game/Inventory.h"
#include "ui/InventorySlot.h"
#include "game/messages.h"
#include "game/actions/ConsumeItemAction.h"

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
        auto selection = getCurrentSelection();
        this->clear();
        const auto &inventory = entity.findComponent<Inventory>();

        for (auto &itemSlot : inventory.getHotBarSlots())
        {
            auto slot = std::make_shared<UI::InventorySlot>(nullptr, itemSlot, entity);

            addObject(slot);
        }
        std::dynamic_pointer_cast<UI::InventorySlot>(objects[selection])->setSelected(true);
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
        size_t selection = getCurrentSelection();
        int newSelection = -1;
        if (pInput->isKeyDown(SDLK_1))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[0])->setSelected(true);
            newSelection = 0;
        }
        else if (pInput->isKeyDown(SDLK_2))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[1])->setSelected(true);
            newSelection = 1;
        }
        else if (pInput->isKeyDown(SDLK_3))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[2])->setSelected(true);
            newSelection = 2;
        }
        else if (pInput->isKeyDown(SDLK_4))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[3])->setSelected(true);
            newSelection = 3;
        }
        else if (pInput->isKeyDown(SDLK_5))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[4])->setSelected(true);
            newSelection = 4;
        }
        else if (pInput->isKeyDown(SDLK_6))
        {
            clearSelection();
            std::dynamic_pointer_cast<UI::InventorySlot>(objects[5])->setSelected(true);
            newSelection = 5;
        }
        if (newSelection != -1 && newSelection == int(selection))
        {
            auto slot = std::dynamic_pointer_cast<UI::InventorySlot>(objects[newSelection]);
            if (slot->getSlot().item)
            {
                // slot->getSlot().item
                actions::ConsumeItemAction action;
                action.execute(slot->getSlot().item, entity);
            }
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

    size_t Hotbar::getCurrentSelection()
    {

        for (size_t i = 0; i < objects.size(); ++i)
        {
            auto slot = std::dynamic_pointer_cast<UI::InventorySlot>(objects[i]);
            if (slot->isSelected())
            {
                return i;
            }
        }
        return 0;
    }
} // namespace UI
