#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>
#include <map>


std::map<int, String> targets;

/* Forward Declarations */
void serializeConfig(String &jsonString, bool pretty = false);
void dsConfig(JsonObject &json);
void saveConfig();
