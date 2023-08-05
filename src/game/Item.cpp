#include "Item.h"

Item::Item(size_t id,
           ItemType itemType,
           std::string name,
           std::string subTextureName)
    : m_id(id), m_itemType(itemType), m_name(name), m_subTextureName(subTextureName)
{
}

Item::~Item()
{
}

size_t Item::getId()
{
    return m_id;
}

std::string &Item::getName()
{
    return m_name;
}

std::string &Item::getSubTextureName()
{
    return m_subTextureName;
}

ItemType Item::getType()
{
    return m_itemType;
}

bool Item::operator==(const Item &other)
{
    return this->m_id == other.m_id;
}