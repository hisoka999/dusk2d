#include "ItemService.h"
#include <magic_enum.hpp>

namespace services
{
    ItemService::ItemService()
    {
    }

    ItemService::~ItemService()
    {
    }
    std::shared_ptr<Item> ItemService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {

        size_t id = object->getIntValue("id");
        ItemType itemType = magic_enum::enum_cast<ItemType>(std::string_view(object->getStringValue("type"))).value();
        std::string name = object->getStringValue("name");
        std::string subTextureName = object->getStringValue("subTextureName");
        std::string prefab = (object->hasAttribute("prefab")) ? object->getStringValue("prefab") : "";
        return std::make_shared<Item>(id, itemType, name, subTextureName, prefab);
    }

    void ItemService::afterLoad()
    {
        for (auto &item : getData())
        {
            itemMap[item->getId()] = item;
        }
    }
    std::shared_ptr<Item> &ItemService::getItemById(size_t id)
    {
        return itemMap.at(id);
    }

    std::shared_ptr<Item> ItemService::getItemByName(const std::string &name)
    {
        for (auto &item : getData())
        {
            if (item->getName() == name)
                return item;
        }
        return nullptr;
    }

} // namespace services
