#include "PlayerHUD.h"
#include <engine/ui/Label.h>
#include <engine/ui/ProgressBar.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/time/time.h>
#include "game/components/Character.h"
#include "game/messages.h"

namespace UI
{

    PlayerHUD::PlayerHUD(/* args */)
    {
        m_characterChangeCallback = core::MessageSystem<MessageType>::get().registerForType(
                MessageType::CHARACTER_UPDATED,
                [this](Character *character)
                {
                    this->m_hpProgressBar->setCurrentValue(character->getHp().getValue());
                    this->m_hpProgressBar->setMaxValue(character->getHp().getMaxValue());
                    this->m_hungerProgressBar->setCurrentValue(character->getHunger().getValue());
                    this->m_thirstProgressBar->setCurrentValue(character->getThirst().getValue());
                    this->m_levelLabel->setText(std::to_string(character->getLevel()));
                });
        m_timeChangeCallback = core::MessageSystem<MessageType>::get().registerForType(
                MessageType::TIME_CHANGED, [this](utils::time::Time time) { m_dateTime->setText(time.format()); });

        addHP();
        addHunger();
        addThirst();

        {
            auto levelLabel = std::make_shared<UI::Label>(nullptr);
            levelLabel->setFont("fonts/arial.ttf", 16);
            levelLabel->setText("Level");
            addObject(levelLabel);
            m_levelLabel = std::make_shared<UI::Label>(nullptr);
            m_levelLabel->setFont("fonts/arial.ttf", 16);
            m_levelLabel->setText("1");
            addObject(m_levelLabel);
            m_dateTime = std::make_shared<UI::Label>(nullptr);
            m_dateTime->setFont("fonts/arial.ttf", 16);
            m_dateTime->setText("");
            addObject(m_dateTime);
        }

        this->setLayout(std::make_shared<UI::layout::GridLayout>(this, 2));
        needsRefresh();
    }

    PlayerHUD::~PlayerHUD() { core::MessageSystem<MessageType>::get().deregister(m_characterChangeCallback); }

    void PlayerHUD::addHP()
    {
        auto levelLabel = std::make_shared<UI::Label>(nullptr);
        levelLabel->setFont("fonts/fa-solid-900.ttf", 16);
        levelLabel->setText("\uf004");

        addObject(levelLabel);

        m_hpProgressBar = std::make_shared<UI::ProgressBar>(nullptr, 200, 20);
        m_hpProgressBar->setCurrentValue(100);
        addObject(m_hpProgressBar);
    }

    void PlayerHUD::addHunger()
    {
        auto label = std::make_shared<UI::Label>(nullptr);
        label->setFont("fonts/fa-solid-900.ttf", 16);
        label->setText("\uf6d7");
        addObject(label);

        m_hungerProgressBar = std::make_shared<UI::ProgressBar>(nullptr, 200, 20);
        m_hungerProgressBar->setCurrentValue(100);
        m_hungerProgressBar->setProgressColor({0, 150, 0, 255});
        addObject(m_hungerProgressBar);
    }

    void PlayerHUD::addThirst()
    {
        auto label = std::make_shared<UI::Label>(nullptr);
        label->setFont("fonts/fa-solid-900.ttf", 16);
        label->setText("\ue4c5");
        addObject(label);

        m_thirstProgressBar = std::make_shared<UI::ProgressBar>(nullptr, 200, 20);
        m_thirstProgressBar->setCurrentValue(100);
        m_thirstProgressBar->setProgressColor({0, 0, 150, 255});

        addObject(m_thirstProgressBar);
    }
    void PlayerHUD::refresh() { endRefresh(); }
} // namespace UI
