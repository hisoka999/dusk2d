#pragma once

#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <vector>
#include <memory>
#include "game/GameMap.h"
#include "ui/windows/PlayerWindow.h"
#include "ui/Hotbar.h"
#include <engine/graphics/texturemap.h>

namespace scenes
{
    class WorldScene : public core::Scene
    {
    private:
        GameMap gameMap;
        std::shared_ptr<UI::PlayerWindow> playerWindow;
        std::unique_ptr<UI::Hotbar> hotBar;
        std::shared_ptr<graphics::TextureMap> itemTextureMap;
        utils::Vector2 mousePos;

    public:
        WorldScene(core::Renderer *pRenderer);
        virtual ~WorldScene();

        void render() override;
        bool handleEvents(core::Input *pInput) override;
        void fixedUpdate(uint32_t delta) override;

        virtual void load();
    };

} // namespace scenes
