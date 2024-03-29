#include "AutotileLayer.h"
#include <algorithm>
#include <engine/core/renderer.h>
#include "GameMap.h"

AutotileLayer::AutotileLayer(size_t width, size_t height,
                             std::vector<std::shared_ptr<graphics::TextureMap>> &textureMaps) :
    width(width),
    height(height), textureMaps(textureMaps)
{
    tiles.resize(width * height, LayerData{});
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
    auto leftUp = getTile(x - 1, y - 1);
    auto rightUp = getTile(x + 1, y - 1);
    auto leftDown = getTile(x - 1, y + 1);
    auto rightDown = getTile(x + 1, y + 1);
    if (left && right && up && down)
    {
        if (!rightDown && leftUp && rightUp && leftDown)
        {
            return hasher("right_down_corner");
        }
        else if (!leftDown && rightDown && rightUp && leftUp)
        {
            return hasher("left_down_corner");
        }
        else if (!leftUp && rightUp && rightDown && leftDown)
        {
            return hasher("left_up_corner");
        }
        else if (!rightUp && leftUp && rightDown && leftDown)
        {
            return hasher("right_up_corner");
        }
        else
        {
            switch (tiles[getTileId(x, y)].data)
            {
                case 0:
                    return hasher("center");
                case 1:
                    return hasher("center1");
                case 2:
                    return hasher("center2");
                case 3:
                    return hasher("center3");
                default:
                    return hasher("center");
                    break;
            }
        }
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

void AutotileLayer::updateIndices()
{
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            const auto id = getTileId(x, y);
            uint8_t tile = tiles[id].tile;
            indexes[id] = calculateIndex(x, y, tile);
            if (x > 0)
                indexes[getTileId(x - 1, y)] = calculateIndex(x - 1, y, getTile(x - 1, y));
            if (x + 1 < width)
                indexes[getTileId(x + 1, y)] = calculateIndex(x + 1, y, getTile(x + 1, y));
            if (y > 0)
                indexes[getTileId(x, y - 1)] = calculateIndex(x, y - 1, getTile(x, y - 1));
            if (y + 1 < height)
                indexes[getTileId(x, y + 1)] = calculateIndex(x, y + 1, getTile(x, y + 1));
        }
    }
}
constexpr size_t AutotileLayer::getTileId(const size_t x, const size_t y) { return (y * width) + x; }
uint8_t AutotileLayer::getTile(size_t x, size_t y)
{
    const auto id = getTileId(x, y);
    if (id >= width * height)
    {
        return 0;
    }
    return tiles[id].tile;
}
void AutotileLayer::setTile(size_t x, size_t y, uint8_t tile)
{
    const auto id = getTileId(x, y);
    tiles[id].tile = tile;
    indexes[id] = calculateIndex(x, y, tile);
    if (x > 0)
        indexes[getTileId(x - 1, y)] = calculateIndex(x - 1, y, getTile(x - 1, y));
    if (x + 1 < width)
        indexes[getTileId(x + 1, y)] = calculateIndex(x + 1, y, getTile(x + 1, y));
    if (y > 0)
        indexes[getTileId(x, y - 1)] = calculateIndex(x, y - 1, getTile(x, y - 1));
    if (y + 1 < height)
        indexes[getTileId(x, y + 1)] = calculateIndex(x, y + 1, getTile(x, y + 1));
}

void AutotileLayer::setTileData(size_t x, size_t y, uint8_t tileData)
{
    const auto id = getTileId(x, y);
    tiles[id].data = tileData;
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
            const auto tileId = tiles[(y * width) + x].tile;
            if (tiles[(y * width) + x].tile == 0)
                continue;

            const auto tileIndice = indexes[(y * width) + x];

            targetRect.x = (x * TILE_SIZE / 2);
            targetRect.y = (y * TILE_SIZE / 2);
            // if (!targetRect.intersects(viewPort))
            //     continue;
            targetRect.x -= renderer->getMainCamera()->getX();
            targetRect.y -= renderer->getMainCamera()->getY();
            textureMaps[tileId - 1]->render(tileIndice, targetRect, renderer);
        }
    }
}

std::vector<core::StaticCollisionBlock> AutotileLayer::generateCollisionMap() const
{
    std::vector<core::StaticCollisionBlock> collider;
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            auto tile = tiles[(y * width) + x];
            switch (tile.tile)
            {
                case 1: // filled
                case 2:
                case 3:
                case 4:
                case 5:
                    std::string blockData =
                            "mountain:" + std::to_string(tile.tile) + ":" + std::to_string(x) + ":" + std::to_string(y);
                    collider.push_back(
                            core::StaticCollisionBlock{.rect = {float(x), float(y), 1, 1}, .blockData = blockData});
                    break;
            }
        }
    }
    return collider;
}
