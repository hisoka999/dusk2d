#pragma once
#include <cstddef>
#include <cstdint>
#include <engine/core/StaticCollisionBlock.h>
#include <engine/graphics/texturemap.h>
#include <memory>
#include <vector>

struct LayerData
{
    uint8_t tile = 0;
    uint8_t data = 0;
};

class AutotileLayer
{
private:
    size_t width;
    size_t height;
    std::vector<LayerData> tiles;
    std::vector<size_t> indexes;
    std::vector<std::shared_ptr<graphics::TextureMap>> textureMaps;
    size_t calculateIndex(size_t x, size_t y, uint8_t tile);
    constexpr size_t getTileId(const size_t x, const size_t y);

public:
    AutotileLayer(size_t width, size_t height, std::vector<std::shared_ptr<graphics::TextureMap>> &textureMaps);
    ~AutotileLayer();
    void updateIndices();
    void setTile(size_t x, size_t y, uint8_t tile);
    void setTileData(size_t x, size_t y, uint8_t tileData);
    uint8_t getTile(size_t x, size_t y);
    void render(core::Renderer *renderer);
    std::vector<core::StaticCollisionBlock> generateCollisionMap() const;
};
