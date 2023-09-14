#pragma once

#include <engine/utils/json/jsonservice.h>
#include "game/Item.h"

namespace services
{
    class ItemService : public utils::json::JSONService<Item>
    {
    private:
        std::map<size_t, std::shared_ptr<Item>> itemMap;
        ItemService(/* args */);
        ~ItemService();
        ItemService(const ItemService &) = delete;
        ItemService &operator=(const ItemService &) = delete;

    protected:
        std::shared_ptr<Item> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);
        void afterLoad() override;

    public:
        static ItemService &Instance()
        {
            static ItemService instance;
            return instance;
        }

        std::shared_ptr<Item> &getItemById(size_t id);
    };

} // namespace services