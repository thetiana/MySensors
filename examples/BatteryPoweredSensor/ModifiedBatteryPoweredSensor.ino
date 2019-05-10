/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2019 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 *
 * This is an example that demonstrates how to report the battery level for a sensor
 * Instructions for measuring battery capacity on A0 are available here:
 * http://www.mysensors.org/build/battery
 *
 * MODIFICATION
 * The modification of the code is written by S.Daskalov and is made for custom made Arduino Mini Shield.
 * Project page https://www.openhardware.io/create/697
 *  
 * FIXED BUGS 
 * Getting battery persentage 102% is fixed.
 */
// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
//#define MY_RADIO_NRF5_ESB
//#define MY_RADIO_RFM69
//#define MY_RADIO_RFM95

#include <MySensors.h>

int sensorPin = A2;    // select the input pin for the voltage divider
int V_FET = 4;      // select the pin for the mosfet control
int Raw_Volt_Value = 0;  // variable to store the value coming from the voltage divider

uint32_t SLEEP_TIME = 900000;  // sleep time between reads (seconds * 1000 milliseconds)
int oldBatteryPcnt = 0;



void setup()
{
    // use the 1.1 V internal reference
#if defined(__AVR_ATmega2560__)
    analogReference(INTERNAL1V1);
#else
    analogReference(INTERNAL);
#endif

pinMode(V_FET, OUTPUT);

}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Modifyed Battery Meter", "1.0");
}

void loop()
{
    // power the voltage divider
    digitalWrite(V_FET, HIGH);
    delay(1);
    // get the battery Voltage
    int Raw_Volt_Value = analogRead(sensorPin);
    // power down voltage divider to extedn battery live
    digitalWrite(V_FET, LOW);
#ifdef MY_DEBUG
    Serial.println(Raw_Volt_Value);
#endif

    int batteryPcnt = Raw_Volt_Value / 10;

  // avoid getting battery persentage 102%
  if(batteryPcnt > 100) {
    batteryPcnt = 100;
    }
  

#ifdef MY_DEBUG
    float batteryV  = Raw_Volt_Value * 0.00415444;
    Serial.print("Battery Voltage: ");
    Serial.print(batteryV);
    Serial.println(" V");

    Serial.print("Battery percent: ");
    Serial.print(batteryPcnt);
    Serial.println(" %");
#endif

    if (oldBatteryPcnt != batteryPcnt) {
        // Power up radio after sleep
        sendBatteryLevel(batteryPcnt);
        oldBatteryPcnt = batteryPcnt;
    }
    sleep(SLEEP_TIME);
}
