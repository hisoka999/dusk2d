#include "CraftingWindow.h"
#include "game/Inventory.h"
#include "translate.h"
#include <engine/graphics/TextureManager.h>
#include "game/services/ItemRecipeService.h"

namespace UI
{
    CraftingWindow::CraftingWindow(int x, int y, const std::string &title) : UI::Window(x, y, 500, 350), m_entity{}
    {
        setTitle(title);
        m_outputProgress = std::make_shared<UI::ProgressBar>(this, 200, 20);
        m_outputProgress->setPos(200, 20);
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

        auto craftingButton = std::make_shared<UI::Button>(this);
        craftingButton->setFont("fonts/arial.ttf", 14);
        craftingButton->setLabel(_("Craft"));
        craftingButton->setPos(20, 260);

        addObject(craftingButton);
        craftingButton->connect(UI::Button::buttonClickCallback(), [this]()
                                {
                                    auto &_inventory = m_entity.findComponent<Inventory>();

                                    // TODO now craft
                                    // move to timer
                                    for(auto recipe : services::ItemRecipeService::Instance().findByRecipeTarget(RecipeTarget::CAMPFIRE)) {
                                        if(_inventory.canCraftRecipe(recipe)) {
                                            //add to queue
                                        }
                                    } });
        // add input
    }

    void CraftingWindow::setEntity(core::ecs::Entity entity)
    {
        m_entity = entity;
        auto &inventory = entity.findComponent<Inventory>();

        for (size_t pos = 0; pos < m_input.size(); ++pos)
        {
            if (m_input[pos] != nullptr)
            {
                removeObject(m_input[pos]);
            }
            auto &itemSlot = inventory.getItemSlots().at(pos);
            m_input[pos] = std::make_shared<UI::InventorySlot>(this, itemSlot, entity);
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

        for (size_t pos = 0; pos < m_fuel.size(); ++pos)
        {
            if (m_fuel[pos] != nullptr)
            {
                removeObject(m_fuel[pos]);
            }
            auto &itemSlot = inventory.getItemSlots().at(pos + m_input.size());
            m_fuel[pos] = std::make_shared<UI::InventorySlot>(this, itemSlot, entity);
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

            addObject(m_fuel[pos]);
        }
    }

    CraftingWindow::~CraftingWindow()
    {
    }

    void CraftingWindow::refresh()
    {
    }
} // namespace UI
