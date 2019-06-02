#ifndef config_h
#define config_h

#include <Arduino.h>


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

    String metarServer;
    std::array<String, 40> leds;
} config_t;


extern config_t config; // Current configuration

#endif