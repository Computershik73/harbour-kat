#include "utils.h"
#include <QTime>

int Utils::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
