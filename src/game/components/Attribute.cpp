#include "Attribute.h"

Attribute::Attribute(int current, int max) : currentValue(current), maxValue(max)
{
}

Attribute::~Attribute()
{
}

void Attribute::addValue(int val)
{
    currentValue += val;
    if (currentValue > maxValue)
    {
        currentValue = maxValue;
    }
    else if (currentValue < 0)
    {
        currentValue = 0;
    }
}

int Attribute::getMaxValue()
{
    return maxValue;
}

int Attribute::getValue()
{
    return currentValue;
}