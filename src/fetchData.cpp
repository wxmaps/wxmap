#include <ESP8266WiFi.h>
#include <asyncHTTPrequest.h>
#include <Ticker.h>
#include "fetchData.h"

asyncHTTPrequest request;
Ticker ticker;

void sendRequest(){
    if(request.readyState() == 0 || request.readyState() == 4){
        char *AIRPORTS = "KBOS,KPVD";

        char url[64] = {0};
        snprintf(url, sizeof(url), "%s%s", BASE_URL, AIRPORTS);

        request.open("GET", url);
        request.send();
    }
}

void requestCB(void* optParm, asyncHTTPrequest* request, int readyState){
    if(readyState == 4){
        Serial.println(request->responseText());
        Serial.println();
        request->setDebug(false);
    }
}
    
void fetchData(wxData_t *data)
{
    request.setDebug(true);
    request.onReadyStateChange(requestCB);
    ticker.attach(5, sendRequest);
}