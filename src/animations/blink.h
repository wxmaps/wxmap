//asdf
#ifndef blink_h
#define blink_h
#include "animationController.h"
#include "animation.h"
#include "arduinoJson.h"

class Blink : public Animation
{
public:
    Blink(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip);

private:
    // Sets the config
    struct AnimationConfig
    {
        RgbColor color;
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
