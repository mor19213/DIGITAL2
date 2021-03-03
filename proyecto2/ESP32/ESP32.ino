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

// uart
#define RX2 16
#define TX2 17
int variable = 0x30;
int value = 0;

// set up the 'counter' feed
AdafruitIO_Feed *eje_x = io.feed("eje_x");

AdafruitIO_Feed *led1 = io.feed("led1");

void setup() {

  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, TX2, RX2);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  led1->onMessage(handleMessage);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  led1->get();
  
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  // UART
  Serial2.write(variable);
  Serial.print("PIC: ");
  Serial.println(Serial2.readString());
  variable++;
  
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(count);
  eje_x->save(count);
  // increment the count by 1
  count++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  if(data->value() == "ON"){
    LUZ1 = 1;
  } else {
    LUZ1 = 0;
  }

}
