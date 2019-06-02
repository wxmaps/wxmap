#include "animationController.h"
#include "animation.h"
#include <ArduinoJson.h>
#include "wind.h"

Wind::Wind(JsonObject &config, NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *stripRef)
{
    animationConfig.duration = config["duration"].as<uint32_t>();
    strip = stripRef;
}

uint32_t Wind::getDuration()
{
    return animationConfig.duration;
}

RgbColor Wind::getColor(uint8_t item)
{
    //Todo
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

void Wind::render(wxData_t *data, float_t progress)
{
    int frame = 0;
    int i = 0;
    for (auto s : data->Wind)
    {
        strip->SetPixelColor(i, getColor(s[frame]));
        i++;
    }
    strip->Show();
}
