#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <engine/core/StaticCollisionBlock.h>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>
#include <vector>
#include "AutotileLayer.h"

typedef uint32_t TileType;

#define TILE_SIZE 64
#define NUMBER_OF_TILES 4

class GameMap
{
private:
    TileType *tiles;
    size_t width;
    size_t height;
    std::shared_ptr<graphics::Texture> texture;
    std::array<std::unique_ptr<AutotileLayer>, NUMBER_OF_TILES> autoTileLayers;

public:
    GameMap(size_t width, size_t height, unsigned int seed);
    size_t getWidth();
    size_t getHeight();
    TileType getTile(size_t x, size_t y);
    void render(core::Renderer *renderer);
    virtual ~GameMap();
    std::vector<core::StaticCollisionBlock> generateCollisionMap() const;
};
