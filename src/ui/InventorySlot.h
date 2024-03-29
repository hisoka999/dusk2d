#include <engine/graphics/texture.h>
#include <engine/graphics/texturemap.h>
#include <engine/ui/Object.h>
#include <engine/ui/StringHint.h>

#include "BaseSlot.h"
#include "game/Inventory.h"

namespace UI
{
    class InventorySlot : public UI::Object, public UI::BaseSlot
    {
    private:
        std::shared_ptr<graphics::Texture> backgroundTexture;
        ItemSlot slot;
        std::shared_ptr<graphics::TextureMap> itemTextureMap;
        std::shared_ptr<graphics::Text> amountText;
        std::shared_ptr<UI::StringHint> hint;

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
        int getAmount() override;
        std::shared_ptr<Item> &getItem() override;
        void updateSlot(std::shared_ptr<Item> item, int amount) override;
    };

} // namespace UI
