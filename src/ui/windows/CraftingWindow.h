#pragma once

#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include <engine/ui/Button.h>
#include <engine/ui/ProgressBar.h>
#include "ui/InventorySlot.h"

namespace UI
{
    class CraftingWindow : public UI::Window
    {
    private:
        std::array<std::shared_ptr<UI::InventorySlot>, 2> m_fuel;
        std::array<std::shared_ptr<UI::InventorySlot>, 4> m_input;
        std::vector<std::shared_ptr<UI::InventorySlot>> m_output;
        std::shared_ptr<UI::ScrollArea> m_outputScrollArea;
        std::shared_ptr<UI::ProgressBar> m_outputProgress;
        core::ecs::Entity m_entity;

    public:
        CraftingWindow(int x, int y, const std::string &title);
        void setEntity(core::ecs::Entity entity);
        ~CraftingWindow();

        void refresh() override;
    };

}