#ifndef animation_h
#define animation_h
#include <Arduino.h>

/* Data structure */
typedef struct
{
    std::array<std::array<uint8_t, 10>, 40> Ceiling;
    std::array<std::array<uint8_t, 10>, 40> Visibility;
    std::array<std::array<uint8_t, 10>, 40> Wind;
    std::array<std::array<uint8_t, 10>, 40> Temperature;
    std::array<std::array<uint8_t, 10>, 40> Sky;
} wxData_t;

class Animation
{
public:
    virtual void render(wxData_t *data, float_t progress);
    //virtual void dsState(JsonObject config);
    virtual uint32_t getDuration();
};



#endif