#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <engine/graphics/texturemap.h>
#include <engine/core/StaticCollisionBlock.h>

class AutotileLayer
{
private:
    size_t width;
    size_t height;
    std::vector<uint8_t> tiles;
    std::vector<size_t> indexes;
    std::vector<std::shared_ptr<graphics::TextureMap>> textureMaps;
    size_t calculateIndex(size_t x, size_t y, uint8_t tile);
    constexpr size_t getTileId(const size_t x, const size_t y);

public:
    AutotileLayer(size_t width, size_t height, std::vector<std::shared_ptr<graphics::TextureMap>> &textureMaps);
    ~AutotileLayer();
    void setTile(size_t x, size_t y, uint8_t tile);
    uint8_t getTile(size_t x, size_t y);
    void render(core::Renderer *renderer);
    std::vector<core::StaticCollisionBlock> generateCollisionMap();
};