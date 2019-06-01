#ifndef animation_h
#define animation_h
#include <Arduino.h>

class Animation
{
public:
    virtual void render(wxData_t data, float_t progress);
    //virtual void dsState(JsonObject config);
    virtual uint32_t getDuration();
};
#endif

/* Data structure */
typedef struct
{
} wxData_t;
