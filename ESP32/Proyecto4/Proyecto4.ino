/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
  https://github.com/luisllamasbinaburo/ESP32-Examples
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Casa";  // Enter your SSID here
const char* password = "Clave123";  //Enter your Password here
// uart
#define RX2 16
#define TX2 17

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

uint8_t LED1pin = 2;
bool LED1status = LOW;
uint8_t var = 0;
uint8_t libres = 4;
uint8_t ocupados = 0;
#define E1 23
#define E2 22
#define E3 21
#define E4 19

#define D1 13
#define D2 12
#define D3 14
#define D4 27
#define D5 26
#define D6 25
#define D7 33
#define D8 32

uint8_t espacio1 = 0;
uint8_t espacio2 = 0;
uint8_t espacio3 = 0;
uint8_t espacio4 = 0;
uint8_t espacios = 0;
uint8_t carro1 = 0;
uint8_t carro2 = 0;
uint8_t carro3 = 0;
uint8_t carro4 = 0;
uint8_t temp1 = 0;
uint8_t temp2 = 0;
uint8_t temp3 = 0;
uint8_t temp4 = 0;

void display(uint8_t);

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(9600);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // serial con tiva
  Serial1.begin(9600, SERIAL_8N1, RX2, TX2);
  
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);
  pinMode(E1, INPUT);
  pinMode(E2, INPUT);
  pinMode(E3, INPUT);
  pinMode(E4, INPUT);
  
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.onNotFound([]() {                  // If the client requests any URI
    if (!HandleFileRead(server.uri()))      // send it if it exists
      handleNotFound();                     // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  //libres = Serial1.read();
  
  carro1 = digitalRead(E1);
  carro2 = digitalRead(E2);
  carro3 = digitalRead(E3);
  carro4 = digitalRead(E4);
  if (carro1 == HIGH){
    espacio1 = 1;
  } else{
    espacio1 = 0;
  }
  if (carro2 == HIGH){
    espacio2 = 1;
  } else{
    espacio2 = 0;
  }
  if (carro3 == HIGH){
    espacio3 = 1;
  } else{
    espacio3 = 0;
  }
  if (carro4 == HIGH){
    espacio4 = 1;
  } else{
    espacio4 = 0;
  }
  libres = espacio1 + espacio2 + espacio3 + espacio4;
  ocupados = 4 - libres;
  //Serial.write(libres+48);
  display(libres);
  server.handleClient();
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status));
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t led1stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Parqueo-matic</title>\n";
  ptr += "<link rel=\"stylesheet\" href=\"styles.css\">\n";
  ptr += "<script>\n";
  ptr += "<!--\n";
  ptr += "function timedRefresh(timeoutPeriod) {\n";
  ptr += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "window.onload = timedRefresh(2500);\n";
  ptr += "\n";
  ptr += "//   -->\n";
  ptr += "</script>";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1> Parqueo-matic </h1>\n";
  ptr += "<br>\n";
  ptr += "<table >\n";
  ptr += "<tr id=\"titulo\">\n";
    ptr += "<th>Parqueo </th>\n";
    ptr += "<th>Estado </th>\n";
  ptr += "</tr>\n";
  ptr += "<tr>\n";
    if (carro1 == HIGH){
      ptr += "<td >Parqueo 1</td>\n";
      ptr += "<td id=\"libre1\"><b>Libre</b></td>\n";
    } else {
      ptr += "<td >Parqueo 1</td>\n";
      ptr += "<td id=\"ocupado1\"><b>Ocupado</b></td>\n";
    }
  ptr += "</tr>\n";
  ptr += "<tr>\n";
    if (carro2 == HIGH){
      ptr += "<td >Parqueo 2</td>\n";
      ptr += "<td id=\"libre2\"><b>Libre</b></td>\n";
    } else {
      ptr += "<td>Parqueo 2</td>\n";
      ptr += "<td id=\"ocupado2\"><b>Ocupado</b></td>\n";
    }
  ptr += "</tr>\n";
  ptr += "<tr>\n";
    if (carro3 == HIGH){
      ptr += "<td>Parqueo 3</td>\n";
      ptr += "<td id=\"libre3\"><b>Libre</b></td>\n";
    } else {
      ptr += "<td>Parqueo 3</td>\n";
      ptr += "<td id=\"ocupado3\"><b>Ocupado</b></td>\n";
    }
  ptr += "</tr>\n";
  ptr += "<tr>\n";
    if (carro4 == HIGH){
      ptr += "<td>Parqueo 4</td>\n";
      ptr += "<td id=\"libre4\"><b>Libre</b></td>\n";
    } else {
      ptr += "<td>Parqueo 4</td>\n";
      ptr += "<td id=\"ocupado4\"><b>Ocupado</b></td>\n";
    }
  ptr += "</tr>\n";
  ptr += "</table>\n";
  ptr += "<br>\n";
    ptr += "<h1>";
  if (carro1 == LOW){
      ptr += "&#128664   ";
    } else {
      ptr += "___   ";
    }
    if (carro2 == LOW){
      ptr += "&#128664   ";
    } else {
      ptr += "___   ";
    }
    if (carro3 == LOW){
      ptr += "&#128664   ";
    } else {
      ptr += "___   ";
    }
    if (carro4 == LOW){
      ptr += "&#128664   ";
    } else {
      ptr += "___   ";
    }
  ptr += "</h1>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void display(uint8_t libres){
  if (libres == 0){ 
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
  }else if (libres == 1){ 
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
  }else if (libres == 2){ 
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);
  }else if (libres == 3){ 
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
  }else if (libres == 4){ 
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
  }
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
