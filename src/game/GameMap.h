#pragma once

#include <cstddef>
#include <cstdint>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>

typedef uint32_t TileType;

#define TILE_SIZE 64

class GameMap
{
private:
    TileType *tiles;
    size_t width;
    size_t height;
    std::shared_ptr<graphics::Texture> texture;

public:
    GameMap(size_t width, size_t height);
    size_t getWidth();
    size_t getHeight();
    size_t getTile(size_t x, size_t y);
    void render(core::Renderer *renderer);
    virtual ~GameMap();
};
