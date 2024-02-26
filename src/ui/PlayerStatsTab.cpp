#include "PlayerStatsTab.h"
#include <engine/ui/Label.h>
#include <engine/ui/layout/GridLayout.h>
#include "game/components/Character.h"
#include "game/messages.h"
#include "translate.h"
namespace UI
{
    PlayerStatsTab::PlayerStatsTab(Object *parent, core::ecs::Entity playerEntity) :
        UI::Tab(parent, _("Stats")), m_playerEntity(playerEntity)
    {
        m_characterChangeCallback = core::MessageSystem<MessageType>::get().registerForType(
                MessageType::CHARACTER_UPDATED, [this](Character *character) { refreshStats(character); });

        this->setLayout(std::make_shared<UI::layout::GridLayout>(this));
    }

    PlayerStatsTab::~PlayerStatsTab() { core::MessageSystem<MessageType>::get().deregister(m_characterChangeCallback); }

    void PlayerStatsTab::refreshStats(Character *character)
    {


        clear();


        for (auto [key, value]: character->displayAttributes())
        {
            auto labelKey = std::make_shared<Label>(this);
            labelKey->setText(key);
            addObject(labelKey);

            auto labelValue = std::make_shared<Label>(this);
            labelValue->setText(value);
            addObject(labelValue);
        }
        updateLayout();
    }

    void PlayerStatsTab::refresh()
    {
        auto character = m_playerEntity.findComponent<Character>();
        refreshStats(&character);
        endRefresh();
    }
} // namespace UI
