#pragma once

#include <engine/core/ecs/Entity.h>
#include <engine/ui/Window.h>

namespace UI
{
    class ChestWindow : public UI::Window
    {
    private:
        core::ecs::Entity m_entity;

    protected:
        void refresh() override;

    public:
        ChestWindow(int x, int y);
        ~ChestWindow();
        void setEntity(core::ecs::Entity entity);
    };

} // namespace UI
