#pragma once

#include <engine/ui/Window.h>
#include <engine/core/ecs/Entity.h>
#include "ui/InventoryComponent.h"
namespace UI
{
    class ScrollArea;

    class PlayerWindow : public UI::Window
    {
    private:
        std::shared_ptr<InventoryComponent> inventoryComponent;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        core::ecs::Entity m_playerEntity;

    protected:
        void refresh() override;

    public:
        PlayerWindow(core::ecs::Entity &entity);
        ~PlayerWindow();
    };

} // namespace UI
