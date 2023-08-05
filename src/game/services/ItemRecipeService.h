#pragma once

#include <engine/utils/json/jsonservice.h>
#include "game/ItemRecipe.h"

namespace services
{
    class ItemRecipeService : public utils::json::JSONService<ItemRecipe>
    {
    private:
        ItemRecipeService(/* args */);
        ~ItemRecipeService();
        ItemRecipeService(const ItemRecipeService &) = delete;
        ItemRecipeService &operator=(const ItemRecipeService &) = delete;

    protected:
        std::shared_ptr<ItemRecipe> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

    public:
        static ItemRecipeService &Instance()
        {
            static ItemRecipeService instance;
            return instance;
        }
    };

} // namespace services
