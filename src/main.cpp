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
#include "config.h"
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <AsyncJson.h>

config_t config;
AnimationController *animCtrl;
AsyncWebServer server(80);

// Configuration Validations TODO
void validateConfig() {}

// De-Serialize Network config
void dsConfig(JsonObject &json)
{
    config.hostname = json["hostname"].as<String>();
    config.ssid = json["ssid"].as<String>();
    config.passphrase = json["passphrase"].as<String>();
    config.gamma = false; //ToDo: remove
    config.pixelCount = 40; //ToDo: remove
    config.metarServer = json["metarServer"].as<String>();
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
    json["metarServer"] = config.metarServer.c_str();

    json["pixelCount"] = static_cast<uint8_t>(config.pixelCount);
    json["gamma"] = config.gamma;

    JsonArray &leds = jsonBuffer.createArray();
    for (auto airport : config.leds){
        leds.add(airport);
    }
    json["leds"] = leds;

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

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        yield();

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
    //ToDo: mdns
    Serial.println(F("+ AnimationController"));
    animCtrl = new AnimationController(config.pixelCount, config.gamma);

    // connects to access point
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.passphrase);
    Serial.print(F(". WiFi"));
    while (WiFi.status() != WL_CONNECTED){
        animCtrl->update();
        yield();
    }
    Serial.printf("\r+ WiFi\ni IP: %s\ni DNS: %s\ni GW: %s\n", WiFi.localIP().toString().c_str(), WiFi.dnsIP().toString().c_str(), WiFi.gatewayIP().toString().c_str());
    animCtrl->setShouldFetch(true);
    Serial.println(F("+ Fetch METAR"));

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.addHandler(new AsyncCallbackJsonWebHandler("/config.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject &jsonObj = json.as<JsonObject>();
        dsConfig(jsonObj);
        saveConfig();
        animCtrl->reloadData();

        request->send(200, "text/plain", "{\"success\": true}");
    }));
    server.addHandler(new AsyncCallbackJsonWebHandler("/mode.json", [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject &jsonObj = json.as<JsonObject>();
        auto id = jsonObj["id"].as<int>();
        animCtrl->queue(id, jsonObj["cfg"]);
        animCtrl->cut();

        request->send(200, "text/plain", "{\"success\": true}");
    }));

    server.onNotFound(notFound);

    server.begin();
}

void loop()
{
    animCtrl->update(); 
}
