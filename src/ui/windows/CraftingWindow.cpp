#include "CraftingWindow.h"
#include <engine/core/ecs/ScriptComponent.h>
#include <engine/graphics/TextureManager.h>
#include "game/Inventory.h"
#include "game/components/CraftingEntity.h"
#include "game/messages.h"
#include "game/services/ItemRecipeService.h"
#include "game/services/ItemService.h"
#include "translate.h"

namespace UI
{
    CraftingWindow::CraftingWindow(int x, int y, const std::string &title) : UI::Window(x, y, 500, 350), m_entity{}
    {
        setTitle(title);
        m_outputProgress = std::make_shared<UI::ProgressBar>(this, 200, 20);
        m_outputProgress->setPos(200, 20);
        m_outputProgress->setMinValue(0);
        m_outputProgress->setMaxValue(100);

        addObject(m_outputProgress);
        m_outputScrollArea = std::make_shared<UI::ScrollArea>(200, 200, this);
        m_outputScrollArea->setPos(200, 40);
        addObject(m_outputScrollArea);
        auto inputLabel = std::make_shared<UI::Label>(this);
        inputLabel->setPos(10, 20);
        inputLabel->setFont("fonts/arial.ttf", 14);
        inputLabel->setStyle(graphics::FontStyle::BOLD);
        inputLabel->setText(_("Input"));
        addObject(inputLabel);

        auto fuelLabel = std::make_shared<UI::Label>(this);
        fuelLabel->setPos(10, 160);
        fuelLabel->setFont("fonts/arial.ttf", 14);
        fuelLabel->setStyle(graphics::FontStyle::BOLD);
        fuelLabel->setText(_("Fuel"));
        addObject(fuelLabel);

        m_craftingButton = std::make_shared<UI::Button>(this);
        m_craftingButton->setFont("fonts/arial.ttf", 14);
        m_craftingButton->setLabel(_("Craft"));
        m_craftingButton->setPos(20, 260);

        addObject(m_craftingButton);
        inventoryRefreshMsgId = core::MessageSystem<MessageType>::get().registerForType(
                MessageType::INVENTORY_UPDATED, [this]([[maybe_unused]] ItemSlot *slot) { this->needsRefresh(); });

        auto craftingCallback = [this](CraftingQueueEntry &entry, bool finished)
        {
            m_outputProgress->setCurrentValue(float(entry.endTime - entry.currentTime) /
                                              float(entry.endTime - entry.startTime) * 100.f);
            this->needsRefresh();
            if (finished)
                m_craftingButton->enable();
        };
        m_craftingButton->connect(UI::Button::buttonClickCallback(),
                                  [this, craftingCallback]()
                                  {
                                      auto crafting = core::ecs::findScriptComponent<CraftingEntity>(m_entity);
                                      crafting->startCrafting();
                                      m_craftingButton->disable();
                                      crafting->progressCallBack(craftingCallback);
                                  });
    }

    void CraftingWindow::setEntity(core::ecs::Entity entity)
    {
        m_entity = entity;
        needsRefresh();
    }

    CraftingWindow::~CraftingWindow() { core::MessageSystem<MessageType>::get().deregister(inventoryRefreshMsgId); }

    void CraftingWindow::refresh()
    {
        auto &inventory = m_entity.findComponent<Inventory>();

        for (size_t pos = 0; pos < m_input.size(); ++pos)
        {
            if (m_input[pos] != nullptr)
            {
                removeObject(m_input[pos]);
            }
            auto &itemSlot = inventory.getHotBarSlots().at(pos);
            m_input[pos] = std::make_shared<UI::InventorySlot>(this, itemSlot, m_entity);
            switch (pos)
            {
                case 0:
                    m_input[pos]->setPos(20, 40);
                    break;
                case 1:
                    m_input[pos]->setPos(80, 40);
                    break;
                case 2:
                    m_input[pos]->setPos(20, 100);
                    break;
                case 3:
                    m_input[pos]->setPos(80, 100);
                    break;
                default:
                    break;
            }

            addObject(m_input[pos]);
        }

        auto dropCallback = [this]([[maybe_unused]] UI::Object *target, std::string &data)
        {
            auto result = utils::split(data, ":");
            int id = std::stoi(result[0]);
            auto item = services::ItemService::Instance().getItemById(id);
            return item->getItemSubType() == ItemSubType::FUEL;
        };

        for (size_t pos = 0; pos < m_fuel.size(); ++pos)
        {
            if (m_fuel[pos] != nullptr)
            {
                removeObject(m_fuel[pos]);
            }
            auto &itemSlot = inventory.getHotBarSlots().at(pos + m_input.size());
            itemSlot.blockedForCrafting = true;
            m_fuel[pos] = std::make_shared<UI::InventorySlot>(this, itemSlot, m_entity);
            switch (pos)
            {
                case 0:
                    m_fuel[pos]->setPos(20, 180);
                    break;
                case 1:
                    m_fuel[pos]->setPos(80, 180);
                    break;
                default:
                    break;
            }
            m_fuel[pos]->setDropCallBack(dropCallback);

            addObject(m_fuel[pos]);
        }

        m_outputScrollArea->clear();
        for (size_t pos = 0; pos < inventory.getItemSlots().size(); ++pos)
        {

            auto &itemSlot = inventory.getItemSlots().at(pos);

            auto slot = std::make_shared<UI::InventorySlot>(m_outputScrollArea.get(), itemSlot, m_entity);
            int x = 20 + ((pos % 2) ? 60 : 0);
            int y = pos / 2 * 60;
            slot->setPos(x, y);

            m_outputScrollArea->addObject(slot);
        }
        endRefresh();
    }
} // namespace UI
