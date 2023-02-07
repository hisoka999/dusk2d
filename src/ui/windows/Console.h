#pragma once

#include <engine/ui/Window.h>
#include <engine/ui/TextItem.h>
#include <engine/ui/scrollarea.h>

namespace UI
{
    class Console : public UI::Window
    {

    public:
        Console();
        ~Console();

    private:
        void initUI();
        std::string executeCommand(std::string value);
        std::shared_ptr<UI::TextItem> textItem;
        std::shared_ptr<UI::ScrollArea> scrollArea;
    };

} // namespace ui
