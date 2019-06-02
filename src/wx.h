#ifndef wx_H
#define wx_H

#include <Arduino.h>
/* Data structure */
typedef struct
{
    bool fetched;
    std::array<std::array<uint8_t, 10>, 40> Ceiling;
    std::array<std::array<uint8_t, 10>, 40> Visibility;
    std::array<std::array<uint8_t, 10>, 40> Wind;
    std::array<std::array<uint8_t, 10>, 40> Temperature;
    std::array<std::array<uint8_t, 10>, 40> Sky;
} wxData_t;

#endif