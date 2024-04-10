#include "ItemService.h"
#include <magic_enum.hpp>
#include <map>
namespace services
{
    ItemService::ItemService() {}

    ItemService::~ItemService() {}
    std::shared_ptr<Item> ItemService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {

        size_t id = object->getIntValue("id");
        ItemType itemType = magic_enum::enum_cast<ItemType>(std::string_view(object->getStringValue("type"))).value();
        ItemSubType itemSubType =
                magic_enum::enum_cast<ItemSubType>(std::string_view(object->getStringValue("subType"))).value();
        std::string name = object->getStringValue("name");
        std::string subTextureName = object->getStringValue("subTextureName");
        std::string prefab = (object->hasAttribute("prefab")) ? object->getStringValue("prefab") : "";
        std::string equipmentTypeString =
                (object->hasAttribute("equipType")) ? object->getStringValue("equipType") : "None";
        EquipmentType equipmentType =
                magic_enum::enum_cast<EquipmentType>(std::string_view(equipmentTypeString)).value();
        std::map<ItemProperty, std::string> values;
        if (object->hasAttribute("data"))
        {
            auto properties = object->getObjectValue("data");
            for (auto key: properties->getAttributes())
            {
                auto propName = magic_enum::enum_cast<ItemProperty>(key).value();
                values[propName] = properties->getStringValue(key);
            }
        }

        return std::make_shared<Item>(id, itemType, itemSubType, equipmentType, name, subTextureName, prefab, values);
    }

    void ItemService::afterLoad()
    {
        for (auto &item: getData())
        {
            itemMap[item->getId()] = item;
        }
    }
    std::shared_ptr<Item> &ItemService::getItemById(size_t id) { return itemMap.at(id); }

    std::shared_ptr<Item> ItemService::getItemByName(const std::string &name)
    {
        for (auto &item: getData())
        {
            if (item->getName() == name)
                return item;
        }
        return nullptr;
    }

    bool ItemService::hasItemForPrefab(const std::string &name)
    {
        for (auto &item: getData())
        {
            if (item->getPrefab() == name)
                return true;
        }
        return false;
    }

    std::shared_ptr<Item> ItemService::getItemByPrefab(const std::string &name)
    {
        for (auto &item: getData())
        {
            if (item->getPrefab() == name)
                return item;
        }
        return nullptr;
    }

} // namespace services
