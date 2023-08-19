#include "InventorySlot.h"
#include <engine/graphics/TextureManager.h>

namespace UI
{
    InventorySlot::InventorySlot(Object *parent, ItemSlot &slot) : UI::Object(parent), slot(slot)
    {
        backgroundTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        setWidth(backgroundTexture->getWidth());
        setHeight(backgroundTexture->getHeight());
        itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");

        amountText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
    }

    InventorySlot::~InventorySlot()
    {
    }

    void InventorySlot::render(core::Renderer *renderer)
    {
        graphics::Rect rect = displayRect();
        if (selected)
        {
            backgroundTexture->setColorKey(255, 0, 255);
        }
        backgroundTexture->render(renderer, rect.x, rect.y);
        backgroundTexture->setColorKey(255, 255, 255);
        if (slot.item)
        {
            auto childTexture = itemTextureMap->getChildTexture(slot.item->getSubTextureName());
            childTexture->render(rect, renderer);
            std::string text = std::to_string(slot.amount);
            int textW, textH;
            amountText->size(text, &textW, &textH);
            amountText->render(renderer, text, {0, 255, 0, 255}, rect.x + rect.width - textW, rect.y + rect.height - textH);
        }
    }

    bool InventorySlot::handleEvents(core::Input *pInput)
    {
        graphics::Rect rect = eventRect();

        utils::Vector2 pos = pInput->getMousePostion();
        bool eventHandled = false;
        if (rect.intersects(pos))
        {

            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {

                eventHandled = this->fireFuncionCall("buttonClick");
                if (!eventHandled)
                    eventHandled = this->fireFuncionCall("buttonClickPara",
                                                         utils::Vector2(pos.getX() - displayRect().x, pos.getY() - displayRect().y));
            }
        }

        Object::handleEvents(pInput);
        return eventHandled;
    }

    void InventorySlot::setSelected(bool select)
    {
        selected = select;
    }
}