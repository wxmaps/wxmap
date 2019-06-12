// animation controller
#ifndef animationController_h
#define animationController_h
#include <NeoPixelBus.h>
#include "arduinoJson.h"
#include "animation.h"
#include "wx.h"
#include "poller.h"

class AnimationController
{
public:
    AnimationController(uint16_t pixelCount, bool gamma);
    void update();
    void cut();
    void reloadData();
    void queue(int animationIndex, JsonObject &cfg);
    void setShouldFetch(bool value);
    Animation *animationFactory(int animationIndex, JsonObject &cfg);

private:
    bool gamma;
    bool shouldFetch;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip = NULL;
    wxData_t *wxData = NULL;
    void PixelCountChanged(uint16_t pixelCount);
    DynamicJsonBuffer cfgBuf;
    Animation *currentAnimation;
    Animation *nextAnimation;
    Poller *poller;
};
#endif
