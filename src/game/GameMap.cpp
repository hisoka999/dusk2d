#include "GameMap.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/perlinnoise.h>
#include <cmath>
#include <algorithm>

GameMap::GameMap(size_t width, size_t height, unsigned int seed) : width(width), height(height)
{

    tiles = new TileType[width * height];

    utils::PerlinNoise noise(seed);

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            double base = 5;
            double value = NUMBER_OF_TILES * noise.noise(base * x / width, base * y / height, 0.5);
            tiles[(y * width) + x] = std::min(value, 3.0);
        }
    }
    texture = graphics::TextureManager::Instance().loadTexture("images/Tileset.png");
}

std::vector<core::StaticCollisionBlock> GameMap::generateCollisionMap()
{
    std::vector<core::StaticCollisionBlock> collider;
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto tile = tiles[(y * width) + x];
            switch (tile)
            {
            case 0: // water
                std::string blockData = "tile:" + std::to_string(tile) + ":" + std::to_string(x) + ":" + std::to_string(y);
                collider.push_back(core::StaticCollisionBlock{.rect = {float(x), float(y), 1, 1}, .blockData = blockData});
                break;
            }
        }
    }
    return collider;
}

size_t GameMap::getWidth()
{
    return width;
}

size_t GameMap::getHeight()
{
    return height;
}

TileType GameMap::getTile(size_t x, size_t y)
{
    return tiles[(y * width) + x];
}

void GameMap::render(core::Renderer *renderer)
{
    graphics::Rect srcRect = {0, 0, TILE_SIZE, TILE_SIZE};
    graphics::Rect targetRect;
    auto viewPort = renderer->getMainCamera()->getViewPortRect();
    int xOffset = std::max(int(viewPort.x / TILE_SIZE * 2), 0);
    int yOffset = std::max(int(viewPort.y / TILE_SIZE * 2), 0);
    int renderWidth = viewPort.width / TILE_SIZE * 2;
    int renderHeight = viewPort.height / TILE_SIZE * 2;
    for (int y = yOffset; y < std::min(yOffset + renderHeight + 2, int(height)); ++y)
    {
        for (int x = xOffset; x < std::min(xOffset + renderWidth + 1, int(width)); ++x)
        {

            targetRect.width = TILE_SIZE / 2;
            targetRect.height = TILE_SIZE / 2;

            TileType tile = tiles[(y * width) + x];
            srcRect.x = tile * TILE_SIZE;
            srcRect.y = 0;
            targetRect.x = (x * TILE_SIZE / 2);
            targetRect.y = (y * TILE_SIZE / 2);
            // if (!targetRect.intersects(viewPort))
            //     continue;
            targetRect.x -= renderer->getMainCamera()->getX();
            targetRect.y -= renderer->getMainCamera()->getY();
            texture->render(renderer, srcRect, targetRect);
            //  renderer->setDrawColor(255, 255, 255, 255);
            //  renderer->fillRect(targetRect);
            //  renderer->setDrawColor(0, 255, 255, 255);
            //  renderer->drawRect(targetRect);
        }
    }
}

GameMap::~GameMap()
{
    delete tiles;
}
