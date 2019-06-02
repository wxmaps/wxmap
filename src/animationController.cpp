#include <NeoPixelBus.h>
#include "animationController.h"
#include "animations/blink.h"
#include "animations/ceiling.h"
#include "ArduinoJson.h"
#include "fetchData.h"

AnimationController::AnimationController(uint16_t pixelCountIn, bool gammaSetting)
{
    wxData = new wxData_t{};
    shouldFetch = false;
    Serial.println(F("AnimationController::AnimationController(...): Call"));
    gamma = gammaSetting;
    Serial.println(F("AnimationController::AnimationController(...): init strip"));
    PixelCountChanged(pixelCountIn);

    Serial.println(F("AnimationController::AnimationController(...): gen test config"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["duration"] = 1000;

    Serial.println(F("AnimationController::AnimationController(...): init blink"));
    queue(new Ceiling(root, strip));
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
    delay(100);
}

void AnimationController::update()
{
    float_t animProg = (millis() % currentAnimation->getDuration()) / (float_t)currentAnimation->getDuration();
    currentAnimation->render(wxData, animProg);
}

void AnimationController::setShouldFetch(bool value)
{
    shouldFetch = value;
    if (shouldFetch)
        fetchData(wxData);
}

void AnimationController::queue(Animation *newAnimation)
{
    Serial.println(F("AnimationController::queue(...)"));
    nextAnimation = newAnimation;
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
    //blink
    case 0:
    {
        return new Blink(cfg, strip);
        break;
    }
    //ceil
    case 1:
    {
        return new Ceiling(cfg, strip);
        break;
    }
    }
    return NULL;
}