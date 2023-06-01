/*
Script para conectar esp8266 a unha WiFi móbil propia



  */
#include <ESP8266WiFi.h>

*//Wifi móbil
#define MAX_INTENTOS 50
#define WIFI_SSID "ssd"
#define WIFI_PASS "pass"

// Pins datos
// GPIO14: D5
#define LED 14

int tempo = 500;
bool conectado = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  conectarWiFi();
}

// Función que se encarga de xestionar a conexión á rede
void conectarWiFi() {
  // put your main code here, to run repeatedly:
  WiFi.mode(WIFI_STA); //Indica modo Station (conexion a outro dispositivo)
  WiFi.disconnect(); // Desconecta unha posible conexion previa
  WiFi.begin(WIFI_SSID, WIFI_PASS); // Inicia a conexion
  Serial.print("\n\nAgardando pola WiFi ");
  int contador = 0; // Comproba estado da conexión e fai varias tentativas
  while(WiFi.status() != WL_CONNECTED and contador < MAX_INTENTOS) {
    contador++;
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // Informa do estado da conexion e IP en caso de exito
  if(contador < MAX_INTENTOS) {
    Serial.print("Conectado á WiFi coa IP: "); Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("Non se puido conectar á WiFi");
  }
}
