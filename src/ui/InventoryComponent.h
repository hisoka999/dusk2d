#include <engine/ui/Container.h>
#include <engine/core/ecs/Entity.h>

namespace UI
{

    class InventoryComponent : public UI::Container, public UI::Object
    {
    private:
        core::ecs::Entity entity;

    public:
        InventoryComponent(UI::Object *parent, const core::ecs::Entity &entity);
        ~InventoryComponent();

        void render(core::Renderer *pRender);
        void postRender(core::Renderer *pRender);
        bool handleEvents(core::Input *pInput);
        void refresh();
    };

} // namespace UI
