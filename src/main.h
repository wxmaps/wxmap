#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>
#include <map>

typedef struct {
    /* Network */
    String ssid;
    String passphrase;
    String hostname;
} config_t;

std::map<int, String> targets;


void dsConfig(JsonObject &json);
