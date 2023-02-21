#include "Sprite.h"
#include <engine/graphics/TextureManager.h>

Sprite::Sprite(const std::string &fileName, size_t rows, size_t cols) : m_rows(rows), m_cols(cols), spriteNum(0)
{
    m_texture = graphics::TextureManager::Instance().loadTexture(fileName);

    m_srcRect = {0.f, 0.f, float(m_texture->getWidth()) / float(cols), float(m_texture->getHeight()) / float(rows)};
    m_rect = m_srcRect;

    setDirection(SpriteDirection::RIGHT);
}

Sprite::~Sprite()
{
}
void Sprite::render(core::Renderer *pRender)
{
    m_texture->render(pRender, m_srcRect, m_rect);
    pRender->setDrawColor(255, 0, 0, 255);
    pRender->drawRect(m_rect);
    pRender->setDrawColor(0, 255, 0, 255);

    pRender->drawRect(getCollisionRect());
}

void Sprite::update(uint32_t deltaTime)
{
    if (!m_isMoving)
        return;

    m_lastUpdate += deltaTime;

    if (m_lastUpdate >= 1000.f / float(m_cols * 2))
    {
        spriteNum++;
        if (spriteNum > m_cols - 1)
        {
            spriteNum = 0;
        }
        m_srcRect.x = spriteNum * m_texture->getWidth() / m_cols;
        m_lastUpdate = 0;
    }
}

graphics::Rect &Sprite::getRect()
{
    return m_rect;
}

graphics::Rect Sprite::getCollisionRect()
{
    graphics::Rect tmp = m_rect;
    tmp.x += m_rect.width / 4;
    tmp.width = m_rect.width / 2;
    return tmp;
}

void Sprite::setPosition(int x, int y)
{
    m_rect.x = x;
    m_rect.y = y;
}

void Sprite::setDirection(SpriteDirection dir)
{
    m_direction = dir;
    m_srcRect.y = int(m_direction) * m_texture->getHeight() / m_rows;
}

void Sprite::startAnimation()
{
    m_isMoving = true;
}

void Sprite::stopAnimation()
{
    m_isMoving = false;
    spriteNum = 0;
}

bool Sprite::isMoving()
{
    return m_isMoving;
}
