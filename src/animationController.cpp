#include <NeoPixelBus.h>
#include "animationController.h"
#include "animations/blink.h"
#include "animations/ceiling.h"
#include "animations/temperature.h"
#include "animations/visibility.h"
#include "animations/wind.h"
#include "ArduinoJson.h"
#include "poller.h"

AnimationController::AnimationController(uint16_t pixelCountIn, bool gammaSetting)
{
    wxData = new wxData_t{};
    shouldFetch = false;
    Serial.println(F("DEBUG init poller"));
    poller = new Poller(wxData);
    Serial.println(F("AnimationController::AnimationController(...): Call"));
    gamma = gammaSetting;
    Serial.println(F("AnimationController::AnimationController(...): init strip"));
    PixelCountChanged(pixelCountIn);

    Serial.println(F("AnimationController::AnimationController(...): gen test config"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["duration"] = 1000;

    Serial.println(F("AnimationController::AnimationController(...): init blink"));
    queue(0, root);
    cut();
    //currentAnimation = new Blink(root, strip);
}

void AnimationController::PixelCountChanged(uint16_t pixelCount)
{
    Serial.print(F("AnimationController::PixelCountCh4anged(...): pixelcount is"));
    Serial.println(pixelCount);
    if (strip != NULL)
    {
        Serial.println(F("AnimationController::PixelCountChanged(...): strip is not NULL"));
        delete strip;
    }
    Serial.println(F("AnimationController::PixelCountChanged(...): init strip"));
    strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(pixelCount);
    Serial.println(F("AnimationController::PixelCountChanged(...): begin strip"));
    //TODO: debug
    strip->Begin();
    strip->ClearTo(1);
    strip->Show();
    delay(1000);
}

void AnimationController::update()
{
    float_t animProg = (millis() % currentAnimation->getDuration()) / (float_t)currentAnimation->getDuration();
    currentAnimation->render(wxData, animProg);
}

void AnimationController::reloadData()
{
    wxData = new wxData_t{};
    poller->sendRequest();
}

void AnimationController::setShouldFetch(bool value)
{
    Serial.println(F("AnimationController::setShouldFetch(...)"));
    shouldFetch = value;
    if (shouldFetch)
        poller->start();
}

void AnimationController::queue(int animationIndex, JsonObject &cfg)
{
    Serial.println(F("AnimationController::queue(...)"));
    nextAnimation = animationFactory(animationIndex, cfg);
}

void AnimationController::cut()
{
    Serial.println(F("AnimationController::cut(...)"));
    auto oldAnimation = currentAnimation;
    currentAnimation = nextAnimation;
}
// later: crossfade? new Crossfade(cfg, strip, animA, animB)

Animation *AnimationController::animationFactory(int animationIndex, JsonObject &cfg)
{
    switch (animationIndex)
    {
    //ceil
    case 0:
    {
        return new Ceiling(cfg, strip);
        break;
    }
    //temperature
    case 1:
    {
        return new Temperature(cfg, strip);
        break;
    }
    //visibility
    case 2:
    {
        return new Visibility(cfg, strip);
        break;
    }
    //wind
    case 3:
    {
        return new Wind(cfg, strip);
        break;
    }
    //blink
    case 255:
    {
        return new Blink(cfg, strip);
        break;
    }
    }
    return NULL;
}