#pragma once

#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <vector>
#include <memory>

namespace scenes
{
    class WorldScene : public core::Scene
    {
    private:
        /* data */
    public:
        WorldScene(core::Renderer *pRenderer);
        virtual ~WorldScene();

        void render() override;
        bool handleEvents(core::Input *pInput) override;

        virtual void load();
    };

} // namespace scenes
