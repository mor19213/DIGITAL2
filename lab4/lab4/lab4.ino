/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.
  
  Hardware Required:
  * LaunchPad with an LED
  
  This example code is in the public domain.  
*/
#include <stdint.h>
// most launchpads have a red LED
#define LEDR RED_LED      //PF_1
#define LEDG GREEN_LED    //PF_1
#define LEDB BLUE_LED     //PF_1
// definir pines a usar para jugadores
#define p11 PB_5
#define p12 PB_0
#define p13 PB_1
#define p14 PE_4
#define p15 PE_5
#define p16 PB_4
#define p17 PA_5
#define p18 PA_6
#define p19 PA_7

#define p21 PD_0
#define p22 PD_1
#define p23 PD_2
#define p24 PD_3
#define p25 PE_1
#define p26 PE_2
#define p27 PE_3
#define p28 PB_2
#define p29 PB_3

uint8_t J1, J2, a1, a2, terminado;


  
// the setup routine runs once when you press reset:
void setup() {  
  Serial.begin(9600);              
  // initialize the digital pin as an output.
  pinMode(LEDR, OUTPUT);  
  pinMode(LEDB, OUTPUT);  
  pinMode(LEDG, OUTPUT); 
  // botones
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);

  // salidas para jugadores
  pinMode(p11, OUTPUT);
  pinMode(p12, OUTPUT);
  pinMode(p13, OUTPUT);
  pinMode(p14, OUTPUT);
  pinMode(p15, OUTPUT);
  pinMode(p16, OUTPUT);
  pinMode(p17, OUTPUT);
  pinMode(p18, OUTPUT);
  pinMode(p19, OUTPUT);
  
  pinMode(p21, OUTPUT);
  pinMode(p22, OUTPUT);
  pinMode(p23, OUTPUT);
  pinMode(p24, OUTPUT);
  pinMode(p25, OUTPUT);
  pinMode(p26, OUTPUT);
  pinMode(p27, OUTPUT);
  pinMode(p28, OUTPUT);
  pinMode(p29, OUTPUT);

  // variables en 0
  J1 = 0;
  J2 = 0; 
  a1 = 0;
  a2 = 0;
  // apagar todos los leds
  digitalWrite(p11, LOW);
  digitalWrite(p12, LOW);
  digitalWrite(p13, LOW);
  digitalWrite(p14, LOW);
  digitalWrite(p15, LOW);
  digitalWrite(p16, LOW);
  digitalWrite(p17, LOW);
  digitalWrite(p18, LOW);
  digitalWrite(p19, LOW);
  digitalWrite(p21, LOW);
  digitalWrite(p22, LOW);
  digitalWrite(p23, LOW);
  digitalWrite(p24, LOW);
  digitalWrite(p25, LOW);
  digitalWrite(p26, LOW);
  digitalWrite(p27, LOW);
  digitalWrite(p28, LOW);
  digitalWrite(p29, LOW);
  terminado = 0;
  // iniciar semaforo
  semaforo();  
}

// the loop routine runs over and over again forever:
void loop() {
  // puntaje jugadores
  Serial.print(J1);
  Serial.print("  ");
  Serial.println(J2);
  // si nadie a ganado avanzar
  if (J1 < 10){
    if (J2 < 10){
      avanzar();
    }
  }
    
  leds();
  
}
// mostrar en los leds el puntaje
void leds(void){
  if (J1 == 1){
    digitalWrite(p11, HIGH);
  } else if (J1 == 2){
    digitalWrite(p12, HIGH);
    digitalWrite(p11, LOW);
  } else if (J1 == 3){
    digitalWrite(p13, HIGH);
    digitalWrite(p12, LOW);
  } else if (J1 == 4){
    digitalWrite(p14, HIGH);
    digitalWrite(p13, LOW);
  } else if (J1 == 5){
    digitalWrite(p15, HIGH);
    digitalWrite(p14, LOW);
  } else if (J1 == 6){
    digitalWrite(p16, HIGH);
    digitalWrite(p15, LOW);
  } else if (J1 == 7){
    digitalWrite(p17, HIGH);
    digitalWrite(p16, LOW);
  } else if (J1 == 8){
    digitalWrite(p18, HIGH);
    digitalWrite(p17, LOW);
  } else if (J1 == 9){
    digitalWrite(p19, HIGH);
    digitalWrite(p18, LOW);
    terminado = 1;
  } else if (J1 == 10){
    terminado = 1;
  }

  if (J2 == 1){
    digitalWrite(p21, HIGH);
  } else if (J2 == 2){
    digitalWrite(p22, HIGH);
    digitalWrite(p21, LOW);
  } else if (J2 == 3){
    digitalWrite(p23, HIGH);
    digitalWrite(p22, LOW);
  } else if (J2 == 4){
    digitalWrite(p24, HIGH);
    digitalWrite(p23, LOW);
  } else if (J2 == 5){
    digitalWrite(p25, HIGH);
    digitalWrite(p24, LOW);
  } else if (J2 == 6){
    digitalWrite(p26, HIGH);
    digitalWrite(p25, LOW);
  } else if (J2 == 7){
    digitalWrite(p27, HIGH);
    digitalWrite(p26, LOW);
  } else if (J2 == 8){
    digitalWrite(p28, HIGH);
    digitalWrite(p27, LOW);
  } else if (J2 == 9){
    digitalWrite(p29, HIGH);
    digitalWrite(p28, LOW);
    terminado = 1;
  } else if (J2 == 10){
    terminado = 1;
  }
}
// leer botones para avanzar
void avanzar(void){
  int reading1 = digitalRead(PUSH1);
  
  if (reading1 == LOW){
    // antirebote
    a1 = 1;
    if (terminado == 1){
      // si ya termino y se presiono el boton volver a iniciar
      delay(1000);
      setup();
    }
  } else {
    if (a1 == 1){ 
      // si no ha terminado, avanzar
      J1++;
      a1 = 0;
    }
  }

  
  int reading2 = digitalRead(PUSH2);
  if (reading2 == LOW){
    // antirebote
    a2 = 1;
    if (terminado == 1){
      // si ya termino y se presiono el boton volver a iniciar
      delay(1000);
      setup();
    }
  } else {
    if (a2 == 1){ 
      // si no ha terminado, avanzar
      J2++;
      a2 = 0;
    }
  }
  
}
void semaforo(void){
  // secuencia para semaforo antes de iniciar carrera
  digitalWrite(LEDR, HIGH);   
  digitalWrite(LEDG, LOW);    
  delay(1000);               
  digitalWrite(LEDG, HIGH); 
  digitalWrite(LEDR, HIGH);  
  delay(1000);              
  digitalWrite(LEDG, HIGH);    
  digitalWrite(LEDR, LOW);    
  delay(1000);              
}
