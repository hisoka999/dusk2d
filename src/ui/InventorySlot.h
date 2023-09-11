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

    public:
        InventorySlot(Object *parent, const ItemSlot &slot, core::ecs::Entity entity);
        ~InventorySlot();
        void setSelected(bool select);

        void render(core::Renderer *renderer) override;
        bool handleEvents(core::Input *pInput) override;
    };

} // namespace UI
