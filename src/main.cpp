/*
    Description des pins physiques:
        GPIO 2: Sortie logique, à convertir en 5V avec le transitor puis l'envoyer au relais
        GPIO 4: Entrée DHT22

    Description des pins virtuels Blynk:
        V0: ENTRÉE - Si 1 alors mode HIVER sinon mode ÉTÉ
        V1: ENTRÉE - Température désirée
        V2: SORTIE - Température ambiante
        V3: SORTIE - Hygrométrie
        V4: SORTIE - LED, Si chauffage alors allumée sinon éteinte
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "../include/blynk_auth.h"

#define PIN_DHT 4
#define PIN_RELAY 2


DHT dht(PIN_DHT, DHT22);
BlynkTimer updateRelayTimer;
WidgetLED isHeatingLED(V4);
float desiredTemp = 19;
const float hysteresis = 1;
bool isWinter = false;
bool isHeating = false;

struct DHTMetrics {float temperature; float humidity;};
DHTMetrics readDHT() {
    return DHTMetrics {dht.readTemperature(), dht.readHumidity()};
}

void updateRelay() {
    DHTMetrics metrics = readDHT();
    Blynk.virtualWrite(V2, String(metrics.temperature, 1));
    Blynk.virtualWrite(V3, String(metrics.humidity, 1));

    isHeating = isWinter ? isHeating ? metrics.temperature < desiredTemp + hysteresis/2 : metrics.temperature < desiredTemp - hysteresis/2 : false;
    digitalWrite(PIN_RELAY, isHeating);
    isHeating ? isHeatingLED.on() : isHeatingLED.off();
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

BLYNK_WRITE(V1) {
    desiredTemp = param.asFloat();
    isHeating = false; // Prendre en charge la nouvelle hystérésis
    updateRelay();
}

BLYNK_WRITE(V0) {
    isWinter = param.asInt() == 1;
    updateRelay();
}

void setup() {
    dht.begin();
    pinMode(PIN_RELAY, OUTPUT);
    updateRelay();
    Blynk.begin(BLYNK_TOKEN, WIFI_SSID, WIFI_KEY);
    updateRelayTimer.setInterval(60000L, updateRelay);
}

void loop() {
    Blynk.run();
    updateRelayTimer.run();
}