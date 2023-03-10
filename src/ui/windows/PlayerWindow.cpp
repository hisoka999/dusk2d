#include "PlayerWindow.h"
#include <engine/ui/TabBar.h>
#include <engine/ui/Tab.h>

namespace UI
{
    void PlayerWindow::refresh()
    {
        inventoryComponent->refresh();
    }

    PlayerWindow::PlayerWindow(const core::ecs::Entity &entity)
        : Window(200, 200, 700, 400)
    {
        setTitle("Player Info");

        auto tabBar = std::make_shared<UI::TabBar>(this);
        tabBar->setPos(0, 0);
        tabBar->setWidth(200);
        tabBar->setHeight(400);

        inventoryComponent = std::make_shared<UI::InventoryComponent>(this, entity);
        inventoryComponent->setPos(200, 50);
        addObject(tabBar);
        addObject(inventoryComponent);

        auto playerTab = std::make_shared<UI::Tab>(tabBar.get(), "Character");
        tabBar->addTab(playerTab);

        auto craftingTab = std::make_shared<UI::Tab>(tabBar.get(), "Crafting");
        tabBar->addTab(craftingTab);
        auto statsTab = std::make_shared<UI::Tab>(tabBar.get(), "Stats");
        tabBar->addTab(statsTab);
    }

    PlayerWindow::~PlayerWindow()
    {
    }
}