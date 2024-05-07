#include "ChestComponent.h"
#include "SDL.h"
#include "ui/windows/ChestWindow.h"
#include "ui/windows/PlayerWindow.h"
void ChestComponent::onClick(int button)
{
    if (button == SDL_BUTTON_LEFT)
    {

        auto chestWindow = getEntity().getScene()->getWindowManager()->findContainer<UI::ChestWindow>();
        auto playerWindow = getEntity().getScene()->getWindowManager()->findContainer<UI::PlayerWindow>();
        playerWindow->setVisible(true);
        chestWindow->setVisible(true);
        chestWindow->setEntity(this->getEntity());


        playerWindow->setPos(chestWindow->getX() + chestWindow->getWidth() + 20, chestWindow->getY());
    }
}
