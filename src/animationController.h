// animation controller
#ifndef animationController_h
#define animationController_h
#include <NeoPixelBus.h>
#include "arduinoJson.h"
#include "animation.h"

class AnimationController
{
public:
    AnimationController(uint16_t pixelCount, bool gamma);
    void update();
    void cut();
    void queue(Animation *newAnimation);
    Animation *animationFactory(int animationIndex, JsonObject &cfg);

private:
    bool gamma;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip = NULL;
    wxData_t *wxData = NULL;
    void PixelCountChanged(uint16_t pixelCount);
    Animation *currentAnimation;
    Animation *nextAnimation;
};
#endif
