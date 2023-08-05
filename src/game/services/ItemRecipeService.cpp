#include "ItemRecipeService.h"

namespace services
{
    ItemRecipeService::ItemRecipeService()
    {
    }
    ItemRecipeService::~ItemRecipeService()
    {
    }

    std::shared_ptr<ItemRecipe> ItemRecipeService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {

        std::map<size_t, size_t> input;

        for (auto &value : object->getArray("input"))
        {
            auto &object = std::get<std::shared_ptr<utils::JSON::Object>>(value);
            input[object->getIntValue("itemId")] = object->getIntValue("amount");
        }

        size_t outputId = object->getIntValue("outputId");
        size_t amount = object->getIntValue("amount");
        std::string title = object->getStringValue("title");
        std::string description = object->getStringValue("description");
        return std::make_shared<ItemRecipe>(input, outputId, amount, title, description);
    }
} // namespace services
