#include "GameMap.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/perlinnoise.h>
#include <random>

GameMap::GameMap(size_t width, size_t height, unsigned int seed) : width(width), height(height)

{

    tiles = new TileType[width * height];

    utils::PerlinNoise noise(seed);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> tileData(0, 4);

    std::vector<std::shared_ptr<graphics::TextureMap>> textureMaps;
    textureMaps.push_back(graphics::TextureManager::Instance().loadTextureMap("images/water.json")); // water
    textureMaps.push_back(graphics::TextureManager::Instance().loadTextureMap("images/sand.json")); // sand
    textureMaps.push_back(graphics::TextureManager::Instance().loadTextureMap("images/grass.json")); // grass
    textureMaps.push_back(graphics::TextureManager::Instance().loadTextureMap("images/mountain.json")); // mountain base
    for (size_t i = 0; i < NUMBER_OF_TILES; ++i)
    {
        std::vector<std::shared_ptr<graphics::TextureMap>> textureMaps2;
        textureMaps2.push_back(textureMaps[i]);
        autoTileLayers[i] = std::make_unique<AutotileLayer>(width, height, textureMaps2);
    }

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            double base = 5;
            double value = NUMBER_OF_TILES * noise.noise(base * x / width, base * y / height, 0.5);
            TileType tile = std::min(value, 3.0);
            tiles[(y * width) + x] = tile;

            autoTileLayers[tile]->setTile(x, y, 1);
            if (tile < NUMBER_OF_TILES - 1)
                autoTileLayers[tile]->setTileData(x, y, tileData(gen));
        }
    }
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto tile = getTile(x, y);
            if (tile == NUMBER_OF_TILES - 1)
                continue;
            // check for collision with other tiles
            bool up, down, left, right = false;
            if (x > 0 && getTile(x - 1, y) != tile)
                left = true;
            if (y > 0 && getTile(x, y - 1) != tile)
                up = true;
            if (y + 1 < height && getTile(x, y + 1) != tile)
                down = true;
            if (x + 1 < width && getTile(x + 1, y) != tile)
                right = true;

            if (up || down || left || right)
            {
                autoTileLayers[tile + 1]->setTile(x, y, 1);
            }
        }
    }

    for (auto &layer: autoTileLayers)
    {
        layer->updateIndices();
    }


    // texture = graphics::TextureManager::Instance().loadTexture("images/Tileset.png");
}

std::vector<core::StaticCollisionBlock> GameMap::generateCollisionMap() const
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
                    std::string blockData =
                            "tile:" + std::to_string(tile) + ":" + std::to_string(x) + ":" + std::to_string(y);
                    collider.push_back(
                            core::StaticCollisionBlock{.rect = {float(x), float(y), 1, 1}, .blockData = blockData});
                    break;
            }
        }
    }
    return collider;
}

size_t GameMap::getWidth() { return width; }

size_t GameMap::getHeight() { return height; }

TileType GameMap::getTile(size_t x, size_t y) { return tiles[(y * width) + x]; }

void GameMap::render(core::Renderer *renderer)
{
    for (int i = autoTileLayers.size() - 1; i >= 0; i--)
    {
        autoTileLayers[i]->render(renderer);
    }
    // graphics::Rect srcRect = {0, 0, TILE_SIZE, TILE_SIZE};
    // graphics::Rect targetRect;
    // auto viewPort = renderer->getMainCamera()->getViewPortRect();
    // int xOffset = std::max(int(viewPort.x / TILE_SIZE * 2), 0);
    // int yOffset = std::max(int(viewPort.y / TILE_SIZE * 2), 0);
    // int renderWidth = viewPort.width / TILE_SIZE * 2;
    // int renderHeight = viewPort.height / TILE_SIZE * 2;
    // for (int y = yOffset; y < std::min(yOffset + renderHeight + 2, int(height)); ++y)
    // {
    //     for (int x = xOffset; x < std::min(xOffset + renderWidth + 1, int(width)); ++x)
    //     {

    //         targetRect.width = TILE_SIZE / 2;
    //         targetRect.height = TILE_SIZE / 2;

    //         TileType tile = tiles[(y * width) + x];
    //         srcRect.x = tile * TILE_SIZE;
    //         srcRect.y = 0;
    //         targetRect.x = (x * TILE_SIZE / 2);
    //         targetRect.y = (y * TILE_SIZE / 2);
    //         // if (!targetRect.intersects(viewPort))
    //         //     continue;
    //         targetRect.x -= std::floor(renderer->getMainCamera()->getX());
    //         targetRect.y -= std::floor(renderer->getMainCamera()->getY());
    //         texture->render(renderer, srcRect, targetRect);
    //         //  renderer->setDrawColor(255, 255, 255, 255);
    //         //  renderer->fillRect(targetRect);
    //         //  renderer->setDrawColor(0, 255, 255, 255);
    //         //  renderer->drawRect(targetRect);
    //     }
    // }
}

GameMap::~GameMap() { delete tiles; }
