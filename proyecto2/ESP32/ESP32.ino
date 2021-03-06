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
signed int ang_x = 0;
signed int ang_y = 0;
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

// set up feeds 
AdafruitIO_Feed *eje_x = io.feed("eje_x");
AdafruitIO_Feed *eje_y = io.feed("eje_y");
AdafruitIO_Feed *eje_z = io.feed("eje_z");

AdafruitIO_Feed *led1 = io.feed("led1");
AdafruitIO_Feed *led2 = io.feed("led2");

AdafruitIO_Feed *cuadrante = io.feed("cuadrante");

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

  // feeds para luces piloto 
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
  io.run();
  escribir();                 // recibir por uart los valores del sensor y pasarlos a angulo
  
  eje_x->save(abs(ang_x));    // Enviar valor angulo a eje x
  // delay para interfaz 
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  io.run();
  variable = luces(LUZ1, LUZ2);   // variable para luces piloto
  Serial1.write(variable);        // enviar variable
 
  eje_y->save(abs(ang_y));        // enviar angulo eje y
  // delay para interfaz 
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  io.run();
  variable = luces(LUZ1, LUZ2); // variable para luces piloto
  Serial1.write(variable);      // enviar variable

  
  if (ang_x > 0){               // Determinar en que cuadrante esta la gravedad
    if (ang_y > 0){
      // +,+
      Serial.print("3 ");
      cuadrante->save("3 cuadrante");
    } else if (ang_y < 0){
      // +,-
      Serial.print("4 ");
      cuadrante->save("4 cuadrante");
    } else if (ang_y == 0){
      Serial.print("-y ");
      cuadrante->save("eje -y");
    }
  } else if (ang_x < 0){
    if (ang_y > 0){
      // -,+
      Serial.print("2 ");
      cuadrante->save("2 cuadrante");
    } else if (ang_y < 0){
      // -,-
      Serial.print("1 ");
      cuadrante->save("1 cuadrante");
    } else if (ang_y == 0){
      Serial.print("+y ");
      cuadrante->save("eje +y");
    }
  } else if (ang_x == 0){
    if (ang_y == 90);
    Serial.print("-x ");
      cuadrante->save("eje -x");
  } else if (ang_y == -90 | ang_y == 166){
    Serial.print("+x ");
      cuadrante->save("eje +x");
  }
  
  // delay para interfaz 
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  delay(500);
  escribir();
  io.run();
  variable = luces(LUZ1, LUZ2);   // variable luces piloto 
  Serial1.write(variable);        // enviar variable 
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){ // si se recibe ON poner en 1
    LUZ1 = 1;
    //digitalWrite(LED, HIGH);
  } else {                     // si se recibe OFF poner en 0
    LUZ1 = 0;
    digitalWrite(LED, LOW);
  }
}
void handleMessage2(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){ // si se recibe ON poner en 1
    LUZ2 = 1;
    //digitalWrite(LED, HIGH);
  } else {                     // si se recibe OFF poner en 0
    LUZ2 = 0;
    digitalWrite(LED, LOW);
  }
}

int luces(int LUZ1, int LUZ2){
  int variable;
  variable = 0;
  if (LUZ1 == 1){
    variable = variable + 1;  // sumar 1 para prender bit 0 de la variable 
  }
  if (LUZ2 == 1){
    variable = variable + 2;  // sumar 2 para prender bit 1 de la variable 
  }
  return variable;
}

signed int angulo(signed int eje){
  signed int resultado;
  if ( 0 < eje & eje < 20){     // angulo en cada eje 
      if (eje == 16){
        resultado = 90;
      }  
      if (eje == 15){
        resultado = 85;
      } 
      else  if (2 <= eje & eje <= 7){
        resultado = (eje * 5) + 5;
      } 
      else  if (eje == 1 ){
        resultado = 0;
      } 
    digitalWrite(LED, LOW);
  } else {
    eje = 256 - (256 + eje);
    
    //digitalWrite(LED, HIGH);

  if (0 <= eje & eje <= 7){
    resultado = map(eje, 7, 0, 45, 80);
  } 
  
    if (eje == 16){
      resultado = 0;
    }
    
    if (16 <= eje & eje <= 23){
      resultado = map(eje, 17, 23, 10, 40);
   resultado = resultado * -1;
    }
  if (7 <= eje & eje <= 14){
    resultado = map(eje, 8, 15, 50, 85);
   resultado = resultado * -1;
  } 
  if (eje == 31){
    resultado = 90;
   resultado = resultado * -1;
    }
  }
  
  return resultado;
}

void escribir(void){
  
  Serial1.write(variable + 16); // variable para luces piloto
  //delay(2);
  tempy = Serial1.read();
  delay(20);
  Serial1.write(variable + 32); // variable para luces piloto
  //delay(2);
  tempx = Serial1.read();

  // pasar binarios con signo a numeros negativos 
   
  if (tempx > 127){
    tempx = (128 - (tempx - 128)) * -1;
  }
  if (tempy > 127){
    tempy = (128 - (tempy - 128)) * -1;
  }
  if (tempz > 127){
    tempz = (128 - (tempz - 128)) * -1;
  }

  if (tempx != 0 & tempx != -7 & tempx != -15){
    ejex = tempx;
  }
  if (tempy != 0 & tempy != -7 & tempx != -15){
    ejey = tempy;
  }

  ang_x = angulo(ejex);
  ang_y = angulo(ejey);
  suma = (abs(ang_x) + abs(ang_y));
  if (suma == 0){
    suma = 90;
  }
  ang_x = ang_x * 90 / suma;
  ang_y = ang_y * 90 / suma;

  // escribir en serial el cuadrante o eje 
  if (ang_x > 0){
    if (ang_y > 0){
      // +,+
      Serial.print("3 ");
    } else if (ang_y < 0){
      // +,-
      Serial.print("4 ");
    } else if (ang_y == 0){
      Serial.print("-y ");
    }
  } else if (ang_x < 0){
    if (ang_y > 0){
      // -,+
      Serial.print("2 ");
    } else if (ang_y < 0){
      // -,-
      Serial.print("1 ");
    } else if (ang_y == 0){
      Serial.print("+y ");
    }
  } else if (ang_x == 0){
    if (ang_y == 90);
    Serial.print("-x ");
  } else if (ang_y == 166){
    Serial.print("+x ");
  }
  
  Serial.print(ang_x);
  Serial.print(" ");
  Serial.print(ang_y);
  Serial.print(" ");
  Serial.println();
  
}
