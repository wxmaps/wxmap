#include "animationController.h"
#include "animation.h"
#include <ArduinoJson.h>
#include "ceiling.h"

Ceiling::Ceiling(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *stripRef)
{
    animationConfig.duration = config["duration"].as<uint32_t>();
    strip = stripRef;
}

uint32_t Ceiling::getDuration()
{
    return animationConfig.duration;
}

RgbColor Ceiling::getColor(uint8_t item)
{
    if (item == 0){
        return RgbColor(0, 0, 0);
    } else if (item < 5){
        return RgbColor(255, 0, 255);
    } else if (item <= 10){
        return RgbColor(255, 0, 0);
    } else if (item <= 30){
        return RgbColor(0, 0, 255);
    }
    return RgbColor(0, 255, 0);
}

void Ceiling::render(wxData_t *data, float_t progress)
{
    int frame = 0;
    int i = 0;
    for (auto s : data->Ceiling)
    {
        strip->SetPixelColor(i, getColor(s[frame]));
        i++;
    }
    strip->Show();
}
