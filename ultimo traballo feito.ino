/****
Script para controlar unha placa ESP8266 e conectar a unha WiFi predeterminada que xenerara 
a placa Raspberri

como  se instala IDE de arduino (ou outros programas a traves do terminal)

1 Abrimos terminal
2 sudo apt-get update
3 sudo apt-cache search arduino
4 sudo apt-get install arduino
5 comprobar se esta instalado

Datos:
MQTT
Datos MQTT broker en mqtt.com
URL: public.mqtthq.com

*****/

#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>

//Wifi móbil
#define MAX_INTENTOS 50
#define WIFI_SSID "ssd"
#define WIFI_PASS "pass"

// Servo
#define SERVOPIN 0 //O pin D3 por algunha razon non traballa co nome Wemos
Servo motor;
#define MQTT_PUB_SERVO "wemos/robotica/servo"
#define MQTT_NOME_CLIENTE "Cliente servo"

// Pins datos
// GPIO14: D5
#define LED 14

int tempo = 500;
bool conectado = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Recordar que a velocidade de baulios sexa a mesma que nos da o monitor
  pinMode(LED, OUTPUT);
  motor.attach(SERVOPIN);
  conectado = conectarWiFi();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  if(conectado) escintila(tempo);
  else escintila(tempo/10);
}

void escintila(int espera) {
  digitalWrite(LED, HIGH);
  delay(espera);
  digitalWrite(LED, LOW);
  delay(espera);
}

// Función que se encarga de xestionar a conexión á rede
bool conectarWiFi() { // cambiamos void por bool para que nos responda verdadeiro ou falsopara o que necesitamos poñer un return
  // put your main code here, to run repeatedly:
  WiFi.mode(WIFI_STA); //Indica modo Station (conexion a outro dispositivo) conectase como unha wifi que se conecta a outra wifi
  WiFi.disconnect(); // Desconecta unha posible conexion previa
  WiFi.begin(WIFI_SSID, WIFI_PASS); // Inicia a conexion
  Serial.print("\n\nAgardando pola WiFi "); // \n\ quere decir 2 voltas de carro e decir deixa 2 liñas en branco antes de escribir
  int contador = 0; // Comproba estado da conexión e fai varias tentativas
  while(WiFi.status() != WL_CONNECTED and contador < MAX_INTENTOS) {
    contador++;
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // Informa do estado da conexion e IP en caso de éxito
  if(contador < MAX_INTENTOS) {
    conectado = true;
    Serial.print("Conectado á WiFi coa IP: "); Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("Non se puido conectar á WiFi");
    conectado = false;
  }
  return(conectado); // utilizamolo porque é bool, void non necesita devolucion
}

/* ===========================================================================

    Definicion da función callback local

    É chamada polo método callback do obxecto que descrebe a conexion MQTT, cada vez que un dispositivo
    publica unha mensaxe nova nun canal (topic) oa que esta suscrito este ESP8266.

    Nesta funcion vai tamen a lóxica que fai accionar os actuadores (servo) conforme a mensaxe que 
    reciban no topic ao que estean suscritos.

    Os sensores publican desde a función loop() , non desde esta funcion

==============================================================================*/

void callback(String topic, byte* message, unsigned int len) {
  Serial.print("Nova mensaxe no topic: "); Serial.print(topic);
  Serial.print(". Mensaxe: ");
  String mensaxeTmp = "";
  for(int i=0; i < len; i++) {
    Serial.print((char)message[i]);
    mensaxeTmp += (char)message[i];
  }
  Serial.println();

  //Loxica que se executa ao recibir o payload
  accionarServo(mensaxeTmp);
}

/** 
    Definicion da funcion reconect local (metodo do obxecto 'mqttClient').
    Cada vez que se inicia unha nova interacion do 'loop()', comprovase que existe a conexion
    ao servidor MQTT. Se non é asi, chamase a esta funcion.

    Encargase de:
    *conectar ao servidor MQTT
    *comunicar por saida serie o estado da conexion MQTT
    *suscribir os sensores/actuadores declarados no topic correspondente
    
**/

void reconnect() {
  //Mentres non se reconecta ao servidor MQTT
  while(!espClient.connected()) {
    Serial.print("Tentando conectar ao servidor MQTT...");
    if(mqttClient.conectet(MQTT_NOME_CLIENTE)) {
      Serial.println(" Conectado"); //continuar en minuto 43:56    }
  }
}

void accionarServo(String orde) {
  //comprovamos se hai orde no teclado
 // if(Serial.available()) {
    //orde = Serial.readStringUntil(\n);
    orde.toLowerCase();
    if(orde.equals("esquerda")) posicion = 180;
    else if(orde.equals("dereita")) posicion = 0;
    else if(orde.equals("centro")) posicion = 90;
    else {
      int tmp = orde.toInt();
      if(tmp >= 0 && tmp <= 180) posicion = tmp;
      else posicion = 0;
   // }
  }
motor.write(posicion);
delay(veloc);
}
