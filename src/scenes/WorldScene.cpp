#include "WorldScene.h"

namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer)
        : core::Scene(pRenderer)
    {
    }

    WorldScene::~WorldScene()
    {
    }

    void WorldScene::render()
    {
        renderEntities(renderer);
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        return handleEventsEntities(pInput);
    }

    void WorldScene::load()
    {
    }
}