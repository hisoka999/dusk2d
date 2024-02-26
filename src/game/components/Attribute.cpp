#include "Attribute.h"

Attribute::Attribute(int baseValue) : baseValue(baseValue) {}

Attribute::~Attribute() {}


int Attribute::getValue()
{
    int value = baseValue;
    for (auto &f: modifiers)
    {
        value += f();
    }
    return value;
}

int Attribute::getBaseValue() { return baseValue; }

void Attribute::addModifier(ModifierFunction &func) { modifiers.emplace_back(func); }
