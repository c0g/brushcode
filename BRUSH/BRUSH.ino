#include <TinkerKit.h>

/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>

/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks the status response for success
*/

// create the XBee object
XBee xbee = XBee();

//Create "accelerometer" object of
//TKAccelerometer class.
TKAccelerometer acc(I0, I1); 
int accelX;
int accelY;
//Same with two leds
TKLed Led(O0);

unsigned long deltaT = 1000;

uint8_t payload[] = { 0, 0, 0, 0 };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40B7231D);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();

int state = LOW;

int avgX = 0;
int avgY = 0;

float ravgX = 0;
float ravgY = 0;
float ravgLagX = 0;
float ravgLagY = 0;
float deltaX = 0;
float deltaY = 0;

unsigned long lastTime = millis();
void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {   
  accelX = abs(acc.readX());
  accelY = abs(acc.readY());
  ravgX = ravgX * 0.5 + 0.5 * (float)accelX;
  ravgY = ravgY * 0.5 + 0.5 * (float)accelY;
  
  deltaX = ravgX - ravgLagX;
  deltaY = ravgY - ravgLagY;
  
  ravgLagX = ravgX;
  ravgLagY = ravgY;
  
  if ((millis() - lastTime) > deltaT) {
    lastTime = millis();

//    Led.brightness(accelX- 500);
    payload[0] = (int)deltaX >> 8 & 0xff;
    payload[1] = (int)deltaX & 0xff;
    payload[2] = (int)deltaY >> 8 & 0xff;
    payload[3] = (int)deltaY & 0xff;
    xbee.send(zbTx);
  }
  
  // flash TX indicator
  

  // after sending a tx request, we expect a status response
//  // wait up to half second for the status response
  if (xbee.readPacket(10)) {
    // got a response!
    // should be a znet tx status            	
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
    } else if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
     xbee.getResponse().getZBRxResponse(rx);
        Led.brightness(rx.getData(0));

////      analogWrite(9, 255);
    }
  }
}

