#pragma once

#include <engine/core/ecs/Entity.h>
#include <engine/ui/Window.h>
#include "ui/EquipmentTab.h"
#include "ui/InventoryComponent.h"
#include "ui/PlayerStatsTab.h"

namespace UI
{
    class ScrollArea;

    class PlayerWindow : public UI::Window
    {
    private:
        std::shared_ptr<InventoryComponent> inventoryComponent;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        std::shared_ptr<UI::PlayerStatsTab> playerStatsTab;
        std::shared_ptr<UI::EquipmentTab> equipmentTab;
        core::ecs::Entity m_playerEntity;
        int inventoryRefreshMsgId;

    protected:
        void refresh() override;

    public:
        PlayerWindow(core::ecs::Entity &entity);
        ~PlayerWindow();
    };

} // namespace UI
