#pragma once

#include <engine/ui/Container.h>
namespace UI
{

    class ProgressBar;
    class Label;

    class PlayerHUD : public UI::Container
    {
    private:
        std::shared_ptr<UI::ProgressBar> m_hpProgressBar;
        std::shared_ptr<UI::ProgressBar> m_hungerProgressBar;
        std::shared_ptr<UI::ProgressBar> m_thirstProgressBar;
        std::shared_ptr<UI::Label> m_levelLabel;
        std::shared_ptr<UI::Label> m_dateTime;
        int m_characterChangeCallback;
        int m_timeChangeCallback;

        void addHP();
        void addHunger();
        void addThirst();

    protected:
        virtual void refresh();

    public:
        PlayerHUD(/* args */);
        ~PlayerHUD();
    };

} // namespace UI
