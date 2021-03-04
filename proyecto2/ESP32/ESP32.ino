// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;
int LUZ1 = 0;
int LUZ2 = 0; 
signed int ejex = 0; 
signed int ejey = 0; 
signed int ejez = 0;
signed int tempx = 0; 
signed int tempy = 0; 
signed int tempz = 0; 
int suma = 0;

// uart
#define RX2 16
#define TX2 17
#define LED 2
int variable = 0;
int value = 0;

// set up the 'counter' feed
AdafruitIO_Feed *eje_x = io.feed("eje_x");
AdafruitIO_Feed *eje_y = io.feed("eje_y");
AdafruitIO_Feed *eje_z = io.feed("eje_z");

AdafruitIO_Feed *led1 = io.feed("led1");
AdafruitIO_Feed *led2 = io.feed("led2");

void setup() {
  pinMode(LED, OUTPUT);
  // start the serial connection
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RX2, TX2);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  led1->onMessage(handleMessage1);
  led2->onMessage(handleMessage2);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  led1->get();
  led2->get();
  
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  // UART
  
  Serial.println("Acelerometro: ");
  
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  
  Serial1.write(variable + 16); // variable para luces piloto
  delay(2);
  tempx = Serial1.read();
  Serial1.write(variable + 32); // variable para luces piloto
  delay(2);
  tempy = Serial1.read();
  
  Serial1.write(variable + 64); // variable para luces piloto
  delay(2);
  tempz = Serial1.read();

  
  if (tempx > 127){
    tempx = (128 - (ejex - 128)) * -1;
  }
  if (tempy > 127){
    tempy = (128 - (ejey - 128)) * -1;
  }
  if (tempz > 127){
    tempz = (128 - (ejez - 128)) * -1;
  }

  if (tempx != 0 & tempx != -7){
    ejex = tempx;
  }
  if (tempy != 0 & tempy != -7){
    ejey = tempy;
  }
  variable = luces(LUZ1, LUZ2);
  Serial.print("eje x -> ");
  Serial.println(ejex);
  
  eje_x->save(ejex);
  delay(3000);
  io.run();
  variable = luces(LUZ1, LUZ2);
  Serial1.write(variable);
  
  Serial.print("eje y -> ");
  Serial.println(ejey);
  eje_y->save(ejey);
  delay(3000);
  io.run();
  variable = luces(LUZ1, LUZ2);
  Serial1.write(variable);

  
  Serial.print("eje z -> ");
  Serial.println(ejez);
  eje_z->save(ejez);
  
  Serial.println(abs(ejez) + abs(ejex) + abs(ejey));
  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  delay(3000);
  io.run();
  variable = luces(LUZ1, LUZ2);
  Serial1.write(variable);

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){
    LUZ1 = 1;
    digitalWrite(LED, HIGH);
  } else {
    LUZ1 = 0;
    digitalWrite(LED, LOW);
  }
}
void handleMessage2(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){
    LUZ2 = 1;
    digitalWrite(LED, HIGH);
  } else {
    LUZ2 = 0;
    digitalWrite(LED, LOW);
  }
}

int luces(int LUZ1, int LUZ2){
  int variable;
  variable = 0;
  if (LUZ1 == 1){
    variable = variable + 1;
  }
  if (LUZ2 == 1){
    variable = variable + 2;
  }
  return variable;
}
