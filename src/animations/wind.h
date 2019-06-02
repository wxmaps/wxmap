//asdf
#ifndef wind_h
#define wind_h
#include "animationController.h"
#include "animation.h"
#include "wx.h"
#include "arduinoJson.h"

class Wind : public Animation
{
public:
    Wind(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip);

private:
    // Sets the config
    struct AnimationConfig
    {
        uint32_t duration;
    };
    // stores the config
    AnimationConfig animationConfig;

    // stores the strip reference
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip;

    uint32_t getDuration();
    void render(wxData_t *data, float_t progress);
};
#endif
