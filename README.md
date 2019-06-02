# wxmap
what's poppin' in my airspace? arduino map to display wx for the US

# roadmap
- [x] Set up config.json file to map an LED id to an IATA code
- [x] In firmware, pull config.json to map to LEDs
- [x] Pull + store METAR every 5mins for all configured
- [x] Implement single display mode
- [ ] Extend display modes for all attributes w/ mapping
    - [x] Ceiling
    - [x] Visibility
    - [ ] Temperature
    - [ ] Wind
- [ ] Set up Web UI for configuring
    - [ ] Mode selection interface
    - [x] LED/IATA mapping

# Data Source
AviationWeather.gov is an unauthenticated dataserver that is used by nearly every FAA-based API, so it will be easiest to use that.
https://www.aviationweather.gov/adds/dataserver_current/httpparam?dataSource=metars&requestType=retrieve&format=xml&hoursBeforeNow=3&mostRecentForEachStation=true&stationString=KORD,KJFK,KLAX

# Configurability
Ideally, this project will allow the configuration of any US-based IATA code. This is beneficial because users will be able to choose any FAA VFR sectional map and place their LEDs in custom locations, even aside from Class C/B. 

# config.json
config.json is a default configuration file for the NEUS map. Should the web interface be unconfigured, this will be the fallback option. The current schema for this JSON object is simply 'LEDnum': 'IATA code'.
