#include "ItemRecipeService.h"
#include <magic_enum.hpp>

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
        size_t craftingTime = object->getIntValue("craftingTime");
        size_t amount = object->getIntValue("amount");
        std::string title = object->getStringValue("title");
        RecipeTarget recipeTarget = magic_enum::enum_cast<RecipeTarget>(object->getStringValue("recipeTarget")).value();
        std::string description = object->getStringValue("description");
        return std::make_shared<ItemRecipe>(input, outputId, amount, craftingTime, recipeTarget, title, description);
    }

    std::vector<std::shared_ptr<ItemRecipe>> ItemRecipeService::findByRecipeTarget(RecipeTarget target)
    {
        std::vector<std::shared_ptr<ItemRecipe>> result;
        for (auto recipe : getData())
        {
            if (recipe->getRecipeTarget() == target)
            {
                result.push_back(recipe);
            }
        }
        return result;
    }
} // namespace services
