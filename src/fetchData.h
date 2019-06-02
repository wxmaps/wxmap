#ifndef fetchData_h
#define fetchData_h

#define BASE_URL "http://10.10.10.237:8000/wx/"

#include <NeoPixelBus.h>
#include "arduinoJson.h"
#include "animation.h"

void fetchData(wxData_t *data);

#endif
