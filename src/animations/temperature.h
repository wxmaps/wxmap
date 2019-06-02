//asdf
#ifndef temperature_h
#define temperature_h
#include "animationController.h"
#include "animation.h"
#include "wx.h"
#include "arduinoJson.h"

class Temperature : public Animation
{
public:
    Temperature(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip);

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
