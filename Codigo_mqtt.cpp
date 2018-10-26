/*
Programa de ejemplo para programar un nodeMCU 
conectarlo a un router WIFI y que envie datos

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
/*Comunicacion serial, aqui va a ir conectado al arduino 
O el prototipo que se quiera enviar los datos mediante serial.
*/
SoftwareSerial swSer(13, 15, false, 256);


#define BUILTIN_LED 4 
const char* ssid = "IOT-Lab"; //Nombre de la red WIFI
const char* password = "InnovationLab-987";//Contraseña
const char* mqtt_server = "192.168.1.127";//IP del servidor 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Inicializa led como salida
  Serial.begin(115200);//Velocidad del serial del Monitor serie
   swSer.begin(9600);//Velocidad del serial que va conectado al arduino o prototipo
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'm') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    swSer.write(0x6D);
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // ciclo cuando se requiere re-conectar
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado !!!");
      // Once connected, publish an announcement...
      client.publish("7/status", "0");
      Serial.println("mensaje publicado");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
//se pregunta si se ha recibido un byte por el puerto serial 
//que va conectado al arduino o el prototipo
   if (swSer.available() > 0) {
    // si llego algun dato se guarda en la varible value
    value = swSer.read();
    //se convierte a una cadena el mensaje y se envia por mqtt.
    snprintf (msg, 3, "%ld", value);
    Serial.print("Publish message: ");
   // Serial.println(msg);
    client.publish("7/status", msg);
    delay(500);
      }
      
    }
  

