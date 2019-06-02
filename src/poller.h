#ifndef poller_h
#define poller_h

#define BASE_URL "http://10.10.10.237:8000/wx/"

#include <NeoPixelBus.h>
#include "arduinoJson.h"
#include "wx.h"
#include <asyncHTTPrequest.h>
#include <Ticker.h>

class Poller
{
    public:
        Poller(wxData_t *dataPointer);
        void start();
    private:
        asyncHTTPrequest request;
        Ticker ticker;
        void requestCB(void *optParm, asyncHTTPrequest *request, int readyState);
        void sendRequest();
        wxData_t *_dataPointer;
};

#endif
