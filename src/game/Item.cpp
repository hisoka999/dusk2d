#include "Item.h"

Item::Item(size_t id,
           ItemType itemType,
           std::string name,
           std::string subTextureName,
           std::string prefab)
    : m_id(id), m_itemType(itemType), m_name(name), m_subTextureName(subTextureName), m_prefab(prefab)
{
}

Item::Item(size_t id,
           ItemType itemType,
           std::string name,
           std::string subTextureName,
           std::string prefab,
           std::map<std::string, std::string> &data) : m_id(id), m_itemType(itemType), m_name(name), m_subTextureName(subTextureName), m_prefab(prefab), m_properties(data)
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
std::string &Item::getPrefab()
{
    return m_prefab;
}

std::string Item::getProperty(const std::string &property)
{
    return m_properties.at(property);
}

const std::string Item::getProperty(const std::string &property) const
{
    return m_properties.at(property);
}