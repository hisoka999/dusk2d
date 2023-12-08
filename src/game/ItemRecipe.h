#pragma once

#include <map>
#include <string>
#include <cstddef>

enum class RecipeTarget
{
    INVENTORY,
    CAMPFIRE,
    FURNANCE,
};

class ItemRecipe
{
private:
    std::map<size_t, size_t> m_input;
    size_t m_outputId;
    size_t m_amount;
    size_t m_craftingTime;
    RecipeTarget m_recipeTarget;
    std::string m_title;
    std::string m_description;

public:
    ItemRecipe(std::map<size_t, size_t> input,
               size_t outputId,
               size_t amount,
               size_t craftingTime,
               RecipeTarget recipeTarget,
               std::string title,
               std::string description);
    ~ItemRecipe();
    const std::map<size_t, size_t> &getInput();
    size_t getOutputId();
    size_t getAmount();
    size_t getCraftingTime();
    RecipeTarget getRecipeTarget();
    std::string &getTitle();
    std::string &getDescription();
};
