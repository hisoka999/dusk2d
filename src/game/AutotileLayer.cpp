#include "AutotileLayer.h"
#include <engine/core/renderer.h>
#include <algorithm>
#include "GameMap.h"

AutotileLayer::AutotileLayer(size_t width, size_t height, std::shared_ptr<graphics::TextureMap> &textureMap) : width(width), height(height), textureMap(textureMap)
{
    tiles.resize(width * height, 0);
    indexes.resize(width * height, 0);
}

AutotileLayer::~AutotileLayer()
{
    tiles.clear();
    indexes.clear();
}

size_t AutotileLayer::calculateIndex(size_t x, size_t y, uint8_t tile)
{
    if (!tile)
        return 0;
    std::hash<std::string_view> hasher;
    auto left = getTile(x - 1, y);
    auto right = getTile(x + 1, y);
    auto up = getTile(x, y - 1);
    auto down = getTile(x, y + 1);
    if (left && right && up && down)
    {
        return hasher("center");
    }
    else if (!left && !up && down && right)
    {
        return hasher("left_up");
    }
    else if (!left && up && !down && right)
    {
        return hasher("left_down");
    }
    else if (left && !up && down && !right)
    {
        return hasher("right_up");
    }
    else if (left && up && !down && !right)
    {
        return hasher("right_down");
    }
    else if (!up && !left && down && !right)
    {
        return hasher("down_border");
    }
    else if (up && !left && !down && !right)
    {
        return hasher("up_border");
    }
    else if (up && !left && down && !right)
    {
        return hasher("left_right_border");
    }
    else if (!up && left && !down && !right)
    {
        return hasher("left_border");
    }
    else if (!up && !left && !down && right)
    {
        return hasher("right_border");
    }
    else if (!up && left && !down && right)
    {
        return hasher("up_down_border");
    }
    else if (left && up && down)
    {
        return hasher("right");
    }
    else if (right && up && down)
    {
        return hasher("left");
    }
    else if (up && left && right)
    {
        return hasher("down");
    }
    else if (down && left && right)
    {
        return hasher("up");
    }
    return hasher("rock");
}
constexpr size_t AutotileLayer::getTileId(const size_t x, const size_t y)
{
    return (y * width) + x;
}
uint8_t AutotileLayer::getTile(size_t x, size_t y)
{
    return tiles[getTileId(x, y)];
}
void AutotileLayer::setTile(size_t x, size_t y, uint8_t tile)
{
    const auto id = getTileId(x, y);
    tiles[id] = tile;
    indexes[id] = calculateIndex(x, y, tile);
    indexes[getTileId(x - 1, y)] = calculateIndex(x - 1, y, getTile(x - 1, y));
    indexes[getTileId(x + 1, y)] = calculateIndex(x + 1, y, getTile(x + 1, y));
    indexes[getTileId(x, y - 1)] = calculateIndex(x, y - 1, getTile(x, y - 1));
    indexes[getTileId(x, y + 1)] = calculateIndex(x, y + 1, getTile(x, y + 1));
}

void AutotileLayer::render(core::Renderer *renderer)
{
    // graphics::Rect srcRect = {0, 0, TILE_SIZE, TILE_SIZE};
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
            if (tiles[(y * width) + x] == 0)
                continue;

            auto tile = indexes[(y * width) + x];

            targetRect.x = (x * TILE_SIZE / 2);
            targetRect.y = (y * TILE_SIZE / 2);
            // if (!targetRect.intersects(viewPort))
            //     continue;
            targetRect.x -= renderer->getMainCamera()->getX();
            targetRect.y -= renderer->getMainCamera()->getY();
            textureMap->render(tile, targetRect, renderer);
        }
    }
}

std::vector<core::StaticCollisionBlock> AutotileLayer::generateCollisionMap()
{
    std::vector<core::StaticCollisionBlock> collider;
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto tile = tiles[(y * width) + x];
            switch (tile)
            {
            case 1: // filled
                std::string blockData = "mountain:" + std::to_string(tile) + ":" + std::to_string(x) + ":" + std::to_string(y);
                collider.push_back(core::StaticCollisionBlock{.rect = {float(x), float(y), 1, 1}, .blockData = blockData});
                break;
            }
        }
    }
    return collider;
}
