/*
wxmap. open for all.
made on vacation in Seattle, WA during the 2019 snowpocalypse.
also made at ajvpot's house after.
*/
#include <NeoPixelBus.h>
#include "main.h"
#include "ESP8266WiFi.h"
#include "FS.h"

#define CONFIG_FILE "/config.json"

config_t config; // Current configuration

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
        Serial.println(F("- No configuration file found."));
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
            Serial.println(F("*** Configuration File too large ***"));
            return;
        }

        std::unique_ptr<char[]> buf(new char[size]);
        file.readBytes(buf.get(), size);

        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        if (!json.success())
        {
            Serial.println(F("*** Configuration File Format Error ***"));
            return;
        }

        dsConfig(json);

        Serial.println(F("- Configuration loaded."));
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
        Serial.println(F("*** Error creating configuration file ***"));
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

    Serial.println(F("* Open FS."));
    if (!SPIFFS.begin())
    {
        Serial.println(F("* Format FS..."));
        SPIFFS.format();
        SPIFFS.begin();
    }
    Serial.println(F("* Load config."));
    loadConfig();
    Serial.println(F("* Set hostname."));
    WiFi.hostname(config.hostname);
}

void loop()
{
    //TODO: animation update
}
