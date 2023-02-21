#pragma once

struct MovementDirection
{
    bool left = false;
    bool right = false;
    bool top = false;
    bool bottom = false;

    bool isMoving()
    {
        return left || right || top || bottom;
    }
};