#include "InventorySlot.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/string.h>
#include "game/services/ItemService.h"

namespace UI
{
    InventorySlot::InventorySlot(Object *parent, const ItemSlot &slot, core::ecs::Entity entity) :
        UI::Object(parent), slot(slot), entity(entity)
    {
        setObjectName("InventorySlot");
        backgroundTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        setWidth(backgroundTexture->getWidth());
        setHeight(backgroundTexture->getHeight());
        hint = std::make_shared<UI::StringHint>();
        setHint(hint);
        itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");

        amountText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
        additionalCheckDropCallback = [](UI::Object *, std::string &) { return true; };

        core::CheckDropCallBack checkDropCallback = [this](UI::Object *target, std::string &data) -> bool
        {
            bool result = dynamic_cast<BaseSlot *>(target) != nullptr;
            if (!result)
                return false;

            return this->additionalCheckDropCallback(target, data);
        };
        setCheckDropCallBack(checkDropCallback);
    }

    InventorySlot::~InventorySlot() {}

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
            amountText->render(renderer, text, {0, 255, 0, 255}, rect.x + rect.width - textW,
                               rect.y + rect.height - textH);
            hint->setHintText(slot.item->getName());
        }
        else
            hint->setHintText("");
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
                    eventHandled =
                            this->fireFuncionCall("buttonClickPara", utils::Vector2(pos.getX() - displayRect().x,
                                                                                    pos.getY() - displayRect().y));
            }
        }

        if (!pInput->isDragActive() && rect.intersects(pos) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT) &&
            slot.item)
        {
            std::string data = std::to_string(slot.item->getId()) + ":" + std::to_string(slot.amount);
            core::DragCallBack dragCallback = [this]([[maybe_unused]] UI::Object *source, UI::Object *target,
                                                     const std::string &data, bool failed)
            {
                if (failed)
                {
                    APP_LOG_ERROR("failed dragged data %s finished from type %s", data, target->getObjectName());
                }
                else if (target)
                {
                    APP_LOG_TRACE("dragged data %s finished from type %s", data, target->getObjectName());
                    UI::BaseSlot *targetSlot = dynamic_cast<UI::BaseSlot *>(target);

                    auto result = utils::split(data, ":");
                    int id = std::stoi(result[0]);
                    if (targetSlot == this)
                        return;

                    if (targetSlot->getItem() && targetSlot->getItem()->getId() == static_cast<size_t>(id))
                    {
                        targetSlot->updateSlot(services::ItemService::Instance().getItemById(id),
                                               targetSlot->getAmount() + std::stoi(result[1]));
                        updateSlot(nullptr, 0);
                    }
                    else
                    {
                        updateSlot(targetSlot->getItem(), targetSlot->getAmount());
                        targetSlot->updateSlot(services::ItemService::Instance().getItemById(id), std::stoi(result[1]));
                    }
                }
            };

            pInput->beginDrag(data, this, dragCallback);
        }
        if (pInput->isDragActive())
        {
            selected = pInput->canDropOnTarget(this);
        }

        Object::handleEvents(pInput);
        return eventHandled;
    }

    void InventorySlot::setDropCallBack(core::CheckDropCallBack callback) { additionalCheckDropCallback = callback; }

    void InventorySlot::setSelected(bool select)
    {
        selected = select;
        auto &inventory = entity.findComponent<Inventory>();
        inventory.setSelectedHotbarSlotById(slot.slotId);
    }

    bool InventorySlot::isSelected() { return selected; }

    ItemSlot &InventorySlot::getSlot() { return slot; }


    int InventorySlot::getAmount() { return slot.amount; }
    std::shared_ptr<Item> &InventorySlot::getItem() { return slot.item; }

    void InventorySlot::updateSlot(std::shared_ptr<Item> item, int amount)
    {
        if (amount == 0)
        {
            slot.item = nullptr;
        }
        else
        {
            slot.item = item;
        }
        slot.amount = amount;
        auto &inventory = entity.findComponent<Inventory>();

        inventory.setItemBySlot(slot);
    }
} // namespace UI
