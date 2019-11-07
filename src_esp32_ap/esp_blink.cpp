#include <Arduino.h>
#include "WiFi.h"

const char *ssid = "HSTU";
const char *password = "passcode";


void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
    WiFi.softAP(ssid, password);
}

// the loop function runs over and over again forever
void loop()
{
    Serial.println("serial working");
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    delay(30);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
    delay(30);                      // Wait for two seconds (to demonstrate the active low LED)
}