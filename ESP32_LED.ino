#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* ssid = "EDGATE TECH";
const char* password = "9535724818";
#define IO_USERNAME  "Vishu123"
#define IO_KEY       "aio_tBCY89NXkI1O4u6OF03hKV306fRu"
// Adafruit IO credentials
char *mqttServer = "io.adafruit.com";
int mqttPort = 1883;


void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), IO_USERNAME, IO_KEY  )) {
      Serial.println("Connected.");
      // Subscribe to the topic for LED control
      mqttClient.subscribe(IO_USERNAME "/feeds/LEDswitch");
    }
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wi-Fi");
  pinMode(2, OUTPUT);
  setupMQTT();
}

void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  Serial.println();

  if (String(topic) == IO_USERNAME"/feeds/LEDswitch") {
    Serial.print("Changing LED output to ");
    if (messageTemp == "ON") {
      Serial.println("ON");
      digitalWrite(2, HIGH);
    } else if (messageTemp == "OFF") {
      Serial.println("OFF");
      digitalWrite(2, LOW);
    }
  }
}
