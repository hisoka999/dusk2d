#pragma once

#include <engine/core/Scene.h>
#include <engine/core/SceneManager.h>
#include <engine/core/renderer.h>
#include <engine/graphics/texturemap.h>
#include <memory>
#include <vector>
#include "game/AutotileLayer.h"
#include "game/GameMap.h"
#include "ui/Hotbar.h"
#include "ui/PlayerHUD.h"
#include "ui/windows/ChestWindow.h"
#include "ui/windows/CraftingWindow.h"
#include "ui/windows/PlayerWindow.h"

namespace scenes
{
    class WorldScene : public core::Scene
    {
    private:
        GameMap gameMap;
        std::shared_ptr<UI::PlayerWindow> playerWindow;
        std::unique_ptr<UI::CraftingWindow> craftingWindow;
        std::unique_ptr<UI::ChestWindow> chestWindow;
        std::unique_ptr<UI::Hotbar> hotBar;
        std::shared_ptr<graphics::TextureMap> itemTextureMap;
        std::shared_ptr<graphics::TextureMap> mountainTextureMap;
        std::shared_ptr<graphics::TextureMap> coalTextureMap;
        std::shared_ptr<graphics::TextureMap> ironTextureMap;
        std::shared_ptr<graphics::TextureMap> silverTextureMap;
        std::shared_ptr<graphics::TextureMap> goldTextureMap;
        std::unique_ptr<AutotileLayer> mountainLayer;
        utils::Vector2 mousePos;
        std::unique_ptr<UI::PlayerHUD> playerHUD;
        [[nodiscard]] utils::Vector2 getMouseMapPos() const;
        TileType getTileOnMouse();

    public:
        explicit WorldScene(core::Renderer *pRenderer);
        ~WorldScene() override;

        void render() override;
        bool handleEvents(core::Input *pInput) override;
        void fixedUpdate(uint32_t delta) override;

        void load() override;
    };

} // namespace scenes
