/*
  Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.

  Update 2014 - TMRh20
*/

/**
   Simplest possible example of using RF24Network,

   RECEIVER NODE
   Listens for messages from the transmitter and prints them out.
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(7, 8);               // nRF24L01(+) radio attached using Getting Started board
int statusOfValve=1;
RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 00;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format
void openValve(){
  if(statusOfValve==0){
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    delay(700);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    Serial.println("VALVE OPENED"); 
    statusOfValve=1;
  }else{
    Serial.println("VALVE REMAINED OPENED"); 
  }
}
void closeValve(){
  if(statusOfValve==1){
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    delay(700);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    statusOfValve=0;
    Serial.println("VALVE CLOSED"); 
  }else{
    Serial.println("VALVE REMAINED CLOSED"); 
  }
}
struct payload_t {                  // Structure of our payload
  float temp;
  float humidity;
  float moisture;
};


void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");
  SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));//SPI.begin();
  radio.begin();
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  closeValve();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void) {

  network.update();                  // Check the network regularly


  while ( network.available() ) {     // Is there anything ready for us?

    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print("tempC ");
    Serial.print(payload.temp);
    Serial.print("C and moisture ");
    Serial.print(payload.moisture);
    Serial.print(" and humidity ");
    Serial.print(payload.humidity);
    Serial.println("");
    if(payload.moisture>550){
      openValve();
    }else{
      closeValve();
    }
  }
}

