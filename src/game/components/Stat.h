#pragma once


class Stat
{
private:
    int currentValue;
    int maxValue;

public:
    Stat(int current, int max);
    ~Stat();

    void addValue(int val);
    int getValue();
    int getMaxValue();
};
