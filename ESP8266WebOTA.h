

#include <Arduino.h>

#ifndef _ESP8266WEBOTA_H_
#define _ESP8266WEBOTA_H_

class ESP8266WebOTA
{
public:
    ESP8266WebOTA(void);
    void begin(char *Name, char *Password, int LED = 2);
    bool check(void);
    void print(String Content, bool Right = false);

private:
};

extern ESP8266WebOTA WebOTA; ///< instantiated by default

#endif