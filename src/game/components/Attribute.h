#pragma once

class Attribute
{
private:
    int currentValue;
    int maxValue;

public:
    Attribute(int current, int max);
    ~Attribute();

    void addValue(int val);
    int getValue();
    int getMaxValue();
};