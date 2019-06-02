//asdf
#ifndef ceiling_h
#define ceiling_h
#include "animationController.h"
#include "animation.h"
#include "wx.h"
#include "arduinoJson.h"

class Ceiling : public Animation
{
public:
    Ceiling(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip);

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
