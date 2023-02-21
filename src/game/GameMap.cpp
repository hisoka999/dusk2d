#include "GameMap.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/perlinnoise.h>

GameMap::GameMap(size_t width, size_t height) : width(width), height(height)
{

    tiles = new TileType[width * height];

    utils::PerlinNoise noise(0);

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            double base = 5;
            double value = 5 * noise.noise(base * x / width, base * y / height, 0.5);
            tiles[(y * width) + x] = value;
        }
    }
    texture = graphics::TextureManager::Instance().loadTexture("images/Tileset.png");
}

size_t GameMap::getWidth()
{
    return width;
}

size_t GameMap::getHeight()
{
    return height;
}

size_t GameMap::getTile(size_t x, size_t y)
{
    return tiles[(y * width) + x];
}

void GameMap::render(core::Renderer *renderer)
{
    graphics::Rect srcRect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            graphics::Rect targetRect;
            targetRect.width = TILE_SIZE / 2;
            targetRect.height = TILE_SIZE / 2;

            TileType tile = tiles[(y * width) + x];
            srcRect.x = tile * TILE_SIZE;
            srcRect.y = 0;
            targetRect.x = (x * TILE_SIZE / 2);
            targetRect.y = (y * TILE_SIZE / 2);
            if (!targetRect.intersects(renderer->getMainCamera()->getViewPortRect()))
                continue;
            targetRect.x -= renderer->getMainCamera()->getX();
            targetRect.y -= renderer->getMainCamera()->getY();
            texture->render(renderer, srcRect, targetRect);
            // renderer->setDrawColor(255, 255, 255, 255);
            // renderer->fillRect(targetRect);
            // renderer->setDrawColor(0, 255, 255, 255);
            // renderer->drawRect(targetRect);
        }
    }
}

GameMap::~GameMap()
{
    delete tiles;
}
