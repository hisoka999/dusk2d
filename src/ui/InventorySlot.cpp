#include "InventorySlot.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/string.h>
#include "game/services/ItemService.h"

namespace UI
{
    InventorySlot::InventorySlot(Object *parent, const ItemSlot &slot, core::ecs::Entity entity) : UI::Object(parent), slot(slot), entity(entity)
    {
        setObjectName("InventorySlot");
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

            if (pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
            {

                eventHandled = this->fireFuncionCall("buttonClick");
                if (!eventHandled)
                    eventHandled = this->fireFuncionCall("buttonClickPara",
                                                         utils::Vector2(pos.getX() - displayRect().x, pos.getY() - displayRect().y));
            }
        }

        if (!pInput->isDragActive() && rect.intersects(pos) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT) && slot.item)
        {
            std::string data = std::to_string(slot.item->getId()) + ":" + std::to_string(slot.amount);
            core::DragCallBack dragCallback = [this]([[maybe_unused]] UI::Object *source, UI::Object *target, const std::string &data, bool failed)
            {
                if (failed)
                {
                    APP_LOG_ERROR("failed dragged data %s finished from type %s", data, target->getObjectName());
                }
                else if (target)
                {
                    auto &inventory = entity.findComponent<Inventory>();
                    APP_LOG_ERROR("dragged data %s finished from type %s", data, target->getObjectName());
                    UI::InventorySlot *targetSlot = dynamic_cast<UI::InventorySlot *>(target);
                    this->slot.amount = targetSlot->slot.amount;
                    this->slot.item = targetSlot->slot.item;
                    auto result = utils::split(data, ":");
                    targetSlot->slot.amount = std::stoi(result[1]);
                    int id = std::stoi(result[0]);
                    targetSlot->slot.item = services::ItemService::Instance().getItemById(id);
                    inventory.setItemBySlot(this->slot);
                    inventory.setItemBySlot(targetSlot->slot);
                }
            };

            core::CheckDropCallBack checkDropCallback = [](UI::Object *target) -> bool
            {
                return dynamic_cast<InventorySlot *>(target) != nullptr;
            };

            pInput->beginDrag(data, this, dragCallback, checkDropCallback);
        }
        if (pInput->isDragActive())
        {
            selected = pInput->canDropOnTarget(this);
        }

        Object::handleEvents(pInput);
        return eventHandled;
    }

    void InventorySlot::setSelected(bool select)
    {
        selected = select;
    }

    bool InventorySlot::isSelected()
    {
        return selected;
    }

    ItemSlot &InventorySlot::getSlot()
    {
        return slot;
    }
}