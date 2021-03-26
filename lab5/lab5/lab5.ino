#include <SPI.h>
#include <SD.h>

File root;
File myFile;
int var;
int menu;
int numero;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.setModule(0);
  var = 0;
  numero = 0;
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(32)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);
}

void loop() {
   // leer serial
  menu = Serial.read();
  // si se recibe algo ir a la subrutina para abrir imagenes
  if (menu > 20){
    abrir(menu);
  }
  
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
        var++;
        if(var > 4){
          if (var < 8){
            // se estan imprimiendo los nombres de los archivos
            // enumerarlos
            numero++;
            Serial.println("");
            Serial.print("imagen ");
            Serial.print(numero);
            Serial.println(":");
          }
        }
     }
     entry.close();
   }
}

void abrir(int num){
    // re-open the file for reading:
    // 3 = mario
    // 2 = pacman
    // 1 = yin yang
    if (num == 0x33){
        myFile = SD.open("mario.txt");
        if (myFile) {
          Serial.print("mario.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
    } else if (num == 0x32){
        myFile = SD.open("pacman.txt");
        if (myFile) {
          Serial.print("pacman.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
    }  else if (num == 0x31){
        myFile = SD.open("yin-yang.txt");
        if (myFile) {
          Serial.print("yin-yang.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
    } else {
      // si es un numero distinto, imprimir que no se encontro la imagen
      Serial.println("Imagen no encontrada");
    }
}
