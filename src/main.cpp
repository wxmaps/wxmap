/*
wxmap. open for all.
made on vacation in Seattle, WA during the 2019 snowpocalypse.
also made at home after.
*/

#define CONFIG_FILE "/config.json"
#define COLOR_SATURATION 128

#include <NeoPixelBus.h>
#include "main.h"
#include "ESP8266WiFi.h"
#include "animationController.h"
#include "FS.h"

config_t config; // Current configuration
AnimationController *animCtrl;

// Configuration

// Configuration Validations TODO
void validateConfig() {}

// De-Serialize Network config
void dsConfig(JsonObject &json)
{
    config.hostname = json["hostname"].as<String>();
    config.ssid = json["ssid"].as<String>();
    config.passphrase = json["passphrase"].as<String>();
    config.gamma = json["gamma"];
    config.pixelCount = json["pixelCount"];
    int i = 0;
    for (auto value : json["leds"].as<JsonArray>())
    {
        config.leds[i] = value.as<String>();
        i++;
    }
    Serial.print("Configured airports: ");
    for (const auto &s : config.leds){
        Serial.print(s);
        Serial.print(" ");
    }
    Serial.print("\n");
}

// Load configugration JSON file
void loadConfig()
{
    // Zeroize Config struct
    Serial.println(F("loadConfig(): memset"));
    memset(&config, 0, sizeof(config));

    Serial.println(F("loadConfig(): spiffs open"));
    // Load CONFIG_FILE json. Create and init with defaults if not found
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file)
    {
        Serial.println(F("! No configuration file found."));
        char chipId[7] = {0};
        snprintf(chipId, sizeof(chipId), "%06x", ESP.getChipId());
        config.ssid = "wxmap" + String(chipId);
        config.hostname = "wxmap" + String(chipId);
        config.passphrase = "admin";
        config.pixelCount = 40;
        config.gamma = false;
        saveConfig();
    }
    else
    {
        // Parse CONFIG_FILE json
        size_t size = file.size();
        if (size > 2048)
        {
            Serial.println(F("! Configuration File too large"));
            return;
        }

        std::unique_ptr<char[]> buf(new char[size]);
        file.readBytes(buf.get(), size);

        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        if (!json.success())
        {
            Serial.println(F("! Configuration File Format Error"));
            return;
        }

        dsConfig(json);

        Serial.println(F("+ Configuration loaded."));
    }

    // Validate it
    validateConfig();
}

// Serialize the current config into a JSON string
void serializeConfig(String &jsonString, bool pretty)
{
    // Create buffer and root object
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();

    json["ssid"] = config.ssid.c_str();
    json["passphrase"] = config.passphrase.c_str();
    json["hostname"] = config.hostname.c_str();

    json["pixelCount"] = static_cast<uint8_t>(config.pixelCount);
    json["gamma"] = config.gamma;

    //ToDo: Serialize airports

    if (pretty)
        json.prettyPrintTo(jsonString);
    else
        json.printTo(jsonString);
}

// Save configuration JSON file
void saveConfig()
{
    // Serialize Config
    String jsonString;
    serializeConfig(jsonString, true);

    // Save Config
    File file = SPIFFS.open(CONFIG_FILE, "w");
    if (!file)
    {
        Serial.println(F("!Error creating configuration file"));
        return;
    }
    else
    {
        file.println(jsonString);
        Serial.println(F("* Configuration saved."));
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println(F("+ FS"));
    if (!SPIFFS.begin())
    {
        Serial.println(F("* Format FS..."));
        SPIFFS.format();
        SPIFFS.begin();
    }
    Serial.println(F("+ Config"));
    loadConfig();
    Serial.println(F("+ Hostname"));
    WiFi.hostname(config.hostname);
    Serial.println(F("+ AnimationController"));
    animCtrl = new AnimationController(config.pixelCount, config.gamma);
}

void loop()
{
    animCtrl->update();
}
