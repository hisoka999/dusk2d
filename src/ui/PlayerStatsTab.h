#pragma once

#include <engine/core/ecs/Entity.h>
#include <engine/ui/Tab.h>

class Character;

namespace UI
{
    class PlayerStatsTab : public UI::Tab
    {
    private:
        core::ecs::Entity m_playerEntity;
        int m_characterChangeCallback;
        void refreshStats(Character *character);

    protected:
        void refresh() override;

    public:
        PlayerStatsTab(Object *parent, core::ecs::Entity playerEntity);
        ~PlayerStatsTab();
    };


} // namespace UI
