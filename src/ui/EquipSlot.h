#pragma once


#include <engine/graphics/texture.h>
#include <engine/graphics/texturemap.h>
#include <engine/ui/Object.h>
#include "BaseSlot.h"
#include "game/Inventory.h"
#include "game/components/Character.h"

namespace UI
{
    class EquipSlot : public UI::Object, public UI::BaseSlot
    {
    private:
        std::shared_ptr<graphics::Texture> backgroundTexture;
        EquipmentSlot slot;
        std::shared_ptr<graphics::TextureMap> itemTextureMap;
        bool selected = false;
        core::ecs::Entity entity;
        core::CheckDropCallBack additionalCheckDropCallback = nullptr;

    public:
        EquipSlot(Object *parent, const EquipmentSlot &slot, core::ecs::Entity entity);
        ~EquipSlot();
        void setSelected(bool select);
        bool isSelected();
        EquipmentSlot &getSlot();

        void render(core::Renderer *renderer) override;
        bool handleEvents(core::Input *pInput) override;
        void setDropCallBack(core::CheckDropCallBack callback);

        int getAmount() override;
        std::shared_ptr<Item> &getItem() override;
        void updateSlot(std::shared_ptr<Item> item, int amount) override;
    };

} // namespace UI
