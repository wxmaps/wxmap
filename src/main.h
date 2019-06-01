#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>
#include <map>

/* Configuration structure */
typedef struct
{
    /* Network */
    String ssid;
    String passphrase;
    String hostname;

    /* Strip config */
    uint16_t pixelCount;
    bool gamma; /* Use gamma map? */

    std::array<String, 40> leds;
} config_t;

std::map<int, String> targets;

/* Forward Declarations */
void serializeConfig(String &jsonString, bool pretty = false);
void dsConfig(JsonObject &json);
void saveConfig();
