#ifndef animation_h
#define animation_h
#include <Arduino.h>
#include "wx.h"
class Animation
{
    public:
        virtual void render(wxData_t *data, float_t progress);
        //virtual void dsState(JsonObject config);
        virtual uint32_t getDuration();
};

#endif