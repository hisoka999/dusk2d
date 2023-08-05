#pragma once

#include <map>
#include <string>
#include <cstddef>

class ItemRecipe
{
private:
    std::map<size_t, size_t> m_input;
    size_t m_outputId;
    size_t m_amount;
    std::string m_title;
    std::string m_description;

public:
    ItemRecipe(std::map<size_t, size_t> input,
               size_t outputId,
               size_t amount,
               std::string title,
               std::string description);
    ~ItemRecipe();
    const std::map<size_t, size_t> &getInput();
    size_t getOutputId();
    size_t getAmount();
    std::string &getTitle();
    std::string &getDescription();
};
