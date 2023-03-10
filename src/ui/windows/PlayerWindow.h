#pragma once

#include <engine/ui/Window.h>
#include <engine/core/ecs/Entity.h>
#include "ui/InventoryComponent.h"
namespace UI
{
    class PlayerWindow : public UI::Window
    {
    private:
        std::shared_ptr<InventoryComponent> inventoryComponent;

    protected:
        void refresh() override;

    public:
        PlayerWindow(const core::ecs::Entity &entity);
        ~PlayerWindow();
    };

} // namespace UI
