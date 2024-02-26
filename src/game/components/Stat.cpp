#include "Stat.h"

Stat::Stat(int current, int max) : currentValue(current), maxValue(max) {}

Stat::~Stat() {}

void Stat::addValue(int val)
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

int Stat::getMaxValue() { return maxValue; }

int Stat::getValue() { return currentValue; }
