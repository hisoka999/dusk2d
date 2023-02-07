#ifndef __OPTIONSWINDOW_H__
#define __OPTIONSWINDOW_H__

#include "SaveGameWindow.h"
#include "ui/windows/SettingsWindow.h"
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
namespace UI
{
    class OptionsWindow : public UI::Window
    {
    private:
        /* data */
    public:
        OptionsWindow(int x, int y, std::shared_ptr<utils::IniBase> settings, core::Input *input);
        ~OptionsWindow();

        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);
        void setPreview(SDL_Surface *previewSurface);

    private:
        void saveGame();
        void loadGame();
        UI::SaveGameWindow saveWindow;
        UI::SaveGameWindow loadWindow;
        SDL_Surface *previewSurface;
        SettingsWindow settingsWindow;
    };

} // namespace UI

#endif // __OPTIONSWINDOW_H__