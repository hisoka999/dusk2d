#include "PlayerWindow.h"
#include <engine/ui/Button.h>
#include <engine/ui/Tab.h>
#include <engine/ui/TabBar.h>
#include <engine/ui/scrollarea.h>
#include "game/Inventory.h"
#include "game/messages.h"
#include "game/services/ItemRecipeService.h"
namespace UI
{
    void PlayerWindow::refresh()
    {
        inventoryComponent->refresh();

        scrollArea->clear();

        int yOffset = 0;
        auto &inventory = m_playerEntity.findComponent<Inventory>();

        for (auto &recipe: services::ItemRecipeService::Instance().findByRecipeTarget(RecipeTarget::INVENTORY))
        {

            auto button = std::make_shared<UI::Button>(scrollArea.get());

            scrollArea->addObject(button);
            button->setFont("fonts/arial.ttf", 12);
            button->setLabel(recipe->getTitle());
            button->setPos(5, yOffset);
            button->setStaticWidth(200);
            yOffset += button->getHeight() + 5;

            if (!inventory.canCraftRecipe(recipe))
                button->disable();
            button->connect(UI::Button::buttonClickCallback(),
                            [this, recipe]()
                            {
                                auto &_inventory = m_playerEntity.findComponent<Inventory>();
                                _inventory.craftItem(recipe);
                                this->needsRefresh();
                            });
        }

        scrollArea->reset();
        playerStatsTab->needsRefresh();
        endRefresh();
    }

    PlayerWindow::PlayerWindow(core::ecs::Entity &entity) : Window(200, 200, 800, 400), m_playerEntity(entity)
    {
        setTitle("Player Info");

        auto tabBar = std::make_shared<UI::TabBar>(this, UI::TabDirection::Top);
        tabBar->setPos(300, 0);
        tabBar->setWidth(400);
        tabBar->setHeight(400);
        tabBar->setTabWidth(80);

        inventoryComponent = std::make_shared<UI::InventoryComponent>(this, entity);
        inventoryComponent->setPos(16, 16);
        addObject(tabBar);
        addObject(inventoryComponent);

        auto playerTab = std::make_shared<UI::EquipmentTab>(tabBar.get(), m_playerEntity);
        tabBar->addTab(playerTab);

        auto craftingTab = std::make_shared<UI::Tab>(tabBar.get(), "Crafting");
        tabBar->addTab(craftingTab);

        scrollArea = std::make_shared<UI::ScrollArea>(400, 300, craftingTab.get());
        craftingTab->addObject(scrollArea);

        playerStatsTab = std::make_shared<UI::PlayerStatsTab>(tabBar.get(), m_playerEntity);
        tabBar->addTab(playerStatsTab);

        auto &msgSystem = core::MessageSystem<MessageType>::get();
        inventoryRefreshMsgId = msgSystem.registerForType(
                MessageType::INVENTORY_UPDATED, [this]([[maybe_unused]] ItemSlot *slot) { this->needsRefresh(); });
    }

    PlayerWindow::~PlayerWindow() { core::MessageSystem<MessageType>::get().deregister(inventoryRefreshMsgId); }
} // namespace UI
