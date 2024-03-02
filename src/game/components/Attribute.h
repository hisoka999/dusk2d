#pragma once

#include <functional>

typedef std::function<int()> ModifierFunction;

class Attribute
{
private:
    int baseValue;
    std::vector<ModifierFunction> modifiers;

public:
    Attribute(int baseValue);
    ~Attribute();

    [[nodiscard]] int getValue();
    [[nodiscard]] int getBaseValue();
    void addModifier(const ModifierFunction &func);
    void clearModifier();
};
