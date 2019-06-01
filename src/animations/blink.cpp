#include "animationController.h"
#include "animation.h"
#include <ArduinoJson.h>
#include "blink.h"

Blink::Blink(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *stripRef)
{
    //animationConfig.color = config["color"].as<RgbColor>();;
    animationConfig.duration = config["duration"].as<uint32_t>();
    ;
    strip = stripRef;
}

uint32_t Blink::getDuration()
{
    return animationConfig.duration;
}

void Blink::render(wxData_t data, float_t progress)
{

    if (progress >= 0.5)
    {
        strip->ClearTo(RgbColor(255, 255, 255));
    }
    else
    {
        strip->ClearTo(RgbColor(0, 0, 0));
    }

    strip->Show();
    ;
}
