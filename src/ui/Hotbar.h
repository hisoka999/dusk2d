#pragma once

#include "engine/ui/Container.h"
#include "engine/core/ecs/Entity.h"

namespace UI
{
    class Hotbar : public UI::Container
    {
    private:
        core::ecs::Entity entity;
        bool initSize = false;
        void clearSelection();

    public:
        Hotbar(const core::ecs::Entity &entity);
        ~Hotbar();
        virtual void render(core::Renderer *pRender);
        // virtual void postRender(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);
    };

} // namespace UI
