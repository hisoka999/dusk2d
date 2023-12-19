#include <engine/ui/Object.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/texturemap.h>
#include "game/Inventory.h"

namespace UI
{
    class InventorySlot : public UI::Object
    {
    private:
        std::shared_ptr<graphics::Texture> backgroundTexture;
        ItemSlot slot;
        std::shared_ptr<graphics::TextureMap> itemTextureMap;
        std::shared_ptr<graphics::Text> amountText;
        bool selected = false;
        core::ecs::Entity entity;
        core::CheckDropCallBack additionalCheckDropCallback = nullptr;

    public:
        InventorySlot(Object *parent, const ItemSlot &slot, core::ecs::Entity entity);
        ~InventorySlot();
        void setSelected(bool select);
        bool isSelected();
        ItemSlot &getSlot();

        void render(core::Renderer *renderer) override;
        bool handleEvents(core::Input *pInput) override;
        void setDropCallBack(core::CheckDropCallBack callback);
    };

} // namespace UI
