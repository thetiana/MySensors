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
 * The modification of the code is written by S.Daskalov and is made for sensors powered directly 
 * from battery without using Arduino voltage regulator.
 * The algorithym use VCC library to use build in Arduino voltage sensor.
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

#include <Vcc.h>
const float VccCorrection = 1.0/1.0;  // Measured Vcc by multimeter divided by reported Vcc
Vcc vcc(VccCorrection);

int v = 0;  // variable to store the value coming from the internal volt reverense
int vpersent = 0; //varible to store the persent value of the battery
uint32_t SLEEP_TIME = 900000;  // sleep time between reads (seconds * 1000 milliseconds)
int oldBatteryPcnt = 0;

void setup() {
  
}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Build in Battery Meter", "1.0");
}

void loop() {
   float v = vcc.Read_Volts();  //read battery voltage in volts
   // asumming 4.25V is battery max voltage or 100% and 2.9V is battery min voltage or 0%
   vpersent = map(v, 2.9, 4.25, 0, 100);
   
   #ifdef MY_DEBUG
    Serial.print("Battery Voltage: ");
    Serial.print(v);
    Serial.println(" V");

    Serial.print("Battery percent: ");
    Serial.print(vpersent);
    Serial.println(" %");
   #endif
   if (oldBatteryPcnt != vpersent) {
        // Power up radio after sleep
        sendBatteryLevel(vpersent);
        oldBatteryPcnt = vpersent;
    }
    sleep(SLEEP_TIME);
   
}
