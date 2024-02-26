#pragma once

#include <engine/core/ecs/Entity.h>
#include <engine/ui/Tab.h>

namespace UI
{
    class EquipmentTab : public UI::Tab
    {
    private:
        core::ecs::Entity m_playerEntity;

    public:
        EquipmentTab(Object *parent, core::ecs::Entity playerEntity);
        ~EquipmentTab();
    };


} // namespace UI
