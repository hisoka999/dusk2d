#include "ItemRecipe.h"

ItemRecipe::ItemRecipe(std::map<size_t, size_t> input,
                       size_t outputId,
                       size_t amount,
                       std::string title,
                       std::string description)
    : m_input(input), m_outputId(outputId), m_amount(amount), m_title(title), m_description(description)
{
}

ItemRecipe::~ItemRecipe()
{
}

size_t ItemRecipe::getAmount()
{
    return m_amount;
}

std::string &ItemRecipe::getDescription()
{
    return m_description;
}

const std::map<size_t, size_t> &ItemRecipe::getInput()
{
    return m_input;
}

std::string &ItemRecipe::getTitle()
{
    return m_title;
}

size_t ItemRecipe::getOutputId()
{
    return m_outputId;
}