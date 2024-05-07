#include "ChestWindow.h"
#include "game/Inventory.h"
#include "translate.h"
#include "ui/InventoryComponent.h"

namespace UI
{

    ChestWindow::ChestWindow(int x, int y) : UI::Window(x, y, 400, 350), m_entity{} { setTitle(_("Chest")); }

    void ChestWindow::refresh()
    {
        clear();
        auto m_inventory = std::make_shared<UI::InventoryComponent>(this, m_entity);
        addObject(m_inventory);
        m_inventory->needsRefresh();
        auto button = std::make_shared<UI::Button>(this);
        button->setPos(100, 280);
        button->setLabel(_("Take all"));
        button->connect(UI::Button::buttonClickCallback(),
                        [m_entity = &m_entity, m_inventory]()
                        {
                            auto &inventory = m_entity->findComponent<Inventory>();
                            auto player = m_entity->getScene()->findEntityByName("player");
                            auto &playerInventory = player->findComponent<Inventory>();
                            for (auto &slot: inventory.getItemSlots())
                            {
                                if (slot.item && slot.amount > 0)
                                {
                                    playerInventory.addItem(slot.item, slot.amount);
                                    slot.amount = 0;
                                    slot.item = nullptr;
                                }
                            }
                            m_inventory->needsRefresh();
                        });
        addObject(button);
        endRefresh();
    }

    ChestWindow::~ChestWindow() {}

    void ChestWindow::setEntity(core::ecs::Entity entity)
    {
        m_entity = entity;
        needsRefresh();
    }
} // namespace UI
