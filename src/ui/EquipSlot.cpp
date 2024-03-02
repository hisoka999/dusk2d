#include "EquipSlot.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/string.h>
#include "game/services/ItemService.h"

namespace UI
{
    EquipSlot::EquipSlot(Object *parent, const EquipmentSlot &slot, core::ecs::Entity entity) :
        UI::Object(parent), slot(slot), entity(entity)
    {
        setObjectName("EquipSlot");
        backgroundTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        setWidth(backgroundTexture->getWidth());
        setHeight(backgroundTexture->getHeight());
        itemTextureMap = graphics::TextureManager::Instance().loadTextureMap("images/items.json");

        additionalCheckDropCallback = [](UI::Object *, std::string &) { return true; };

        core::CheckDropCallBack checkDropCallback = [this](UI::Object *target, std::string &data) -> bool
        {
            bool result = dynamic_cast<BaseSlot *>(target) != nullptr;
            if (!result)
                return false;
            auto data_arr = utils::split(data, ":");
            int id = std::stoi(data_arr[0]);

            auto &item = services::ItemService::Instance().getItemById(id);
            if (item->getType() != ItemType::EQUIPMENT)
                return false;
            if (this->getSlot().type != item->getEquipmentType())
                return false;

            return this->additionalCheckDropCallback(target, data);
        };
        setCheckDropCallBack(checkDropCallback);
    }

    EquipSlot::~EquipSlot() {}

    void EquipSlot::render(core::Renderer *renderer)
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
        }
    }

    bool EquipSlot::handleEvents(core::Input *pInput)
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
            std::string data = std::to_string(slot.item->getId()) + ":" + std::to_string(1);
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

    void EquipSlot::setDropCallBack(core::CheckDropCallBack callback) { additionalCheckDropCallback = callback; }

    void EquipSlot::setSelected(bool select) { selected = select; }

    bool EquipSlot::isSelected() { return selected; }

    EquipmentSlot &EquipSlot::getSlot() { return slot; }

    int EquipSlot::getAmount() { return slot.item ? 1 : 0; }

    std::shared_ptr<Item> &EquipSlot::getItem() { return slot.item; }

    void EquipSlot::updateSlot(std::shared_ptr<Item> item, int amount)
    {
        if (amount == 0)
        {
            slot.item = nullptr;
        }
        else
        {
            slot.item = item;
        }
        auto &character = entity.findComponent<Character>();
        character.updateEquipment(slot);
    }
} // namespace UI
