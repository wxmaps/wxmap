/*
wxmap. open for all.
made on vacation in Seattle, WA during the 2019 snowpocalypse.
*/

#include <NeoPixelBus.h>
#include "main.h"
#include "FS.h"

#define CONFIG_FILE "/config.json"

config_t config;

void dsConfig(JsonObject &json) {
    config.hostname = json["hostname"].as<String>();
    if(!config.hostname.length()) {
        config.hostname = "wxmap" + ESP.getChipId());
    }

    config.ssid = json["ssid"].as<String>();
    if(!config.ssid.length()) {
        config.ssid = "wxmap";
    }
    config.passphrase = json["passphrase"].as<String>();
    if(!config.passphrase.length()) {
        config.passphrase = "admin";
    }


}

void loadConfig() {
    Serial.println(F("[i] zeroize memset"));
    memset(&config, 0, sizeof(config));
    Serial.println(F("[i] spiffs open"));
    File file = SPIFFS.open(CONFIG_FILE, "r");

    if(!file) {
        Serial.println(F("[!] No configuration file found. Setting defaults."));
        config.ssid = "wxmap";
        config.passphrase = "admin";
    } else {
        size_t size = file.size();
        if(size > 4096) {
            Serial.println(F("[!!] Configuration file too large!"));
            return;
        }
        std::unique_ptr<char[]> buf(new char[size]);
        file.readBytes(buf.get(), size);

        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        
        if(!json.success()) {
            Serial.println(F("[!!] Configuration file format is incorrect."));
            return;
        }
        dsConfig(json);

        Serial.println(F("[i] Configuration loaded."));
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println(F("[i] Open FS."));
    SPIFFS.begin();
    Serial.println(F("[i] Load config."));
    loadConfig();
    Serial.println(F("[i] Set hostname."));
    WiFi.hostname(config.hostname);
    Serial.println(F("[i] Init WXMap"));   
}