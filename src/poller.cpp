#include "poller.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <asyncHTTPrequest.h>
#include <Ticker.h>
#include "config.h"


Poller::Poller(wxData_t *dataPointer)
{
    _dataPointer = dataPointer;
    request.setDebug(true);
    //ToDo: lol
    //request.onReadyStateChange([this](void* a, asyncHTTPrequest* b, int c){requestCB(a,b,c);});
    request.onReadyStateChange(std::bind(&Poller::requestCB, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void Poller::start()
{
    ticker.attach(5, std::bind(&Poller::sendRequest, this));
}

void Poller::sendRequest()
{
    Serial.println(ESP.getFreeHeap());
    if (request.readyState() == 0 || request.readyState() == 4)
    {
        char url[256] = {0};
        strncat(url, config.metarServer, sizeof(url) - 1);
        int i = 0;
        for (auto airport : config.leds){
            if (airport.length())
            {
                strncat(url, airport.c_str(), sizeof(url) - strlen(url) - 1);
                strncat(url, ",", sizeof(url) - strlen(url) - 1);
            }
            i++;
        }
        // Remove trailing comma
        url[strlen(url)-1] = 0;

        request.open("GET", url);
        request.send();
    }
}

void Poller::requestCB(void *optParm, asyncHTTPrequest *request, int readyState)
{
    if (readyState == 4)
    {
        request->setDebug(false);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(request->responseText());
        if (!json.success())
        {
            Serial.println(F("! bad json"));
            return;
        }
        Serial.println(F("+ parsed json"));

        int airportIdx = 0;
        //[[1,2],[5]]
        for (auto c : json["C"].as<JsonArray>()){
            int frameIdx = 0;
            //[1,2]
            for (auto frame : c.as<JsonArray>())
            {
                //1
                _dataPointer->Ceiling[airportIdx][frameIdx] = frame.as<int>();
                frameIdx++;
            }
            airportIdx++;
        }
    }
}