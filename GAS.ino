// #include "BluetoothSerial.h" 
#include "ArduinoJson.h"
#include "WiFi.h"
#include <PubSubClient.h>

// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>

// BluetoothSerial ESP_BT;

// Pengaturan WiFi
const char* ssid = "Wifi Elite";
const char* password = "yayaya1234";

// Pengaturan MQTT
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;  // Ganti dengan port yang sesuai
const char* clientID = "gasdetectiontelkom";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Koneksi WiFi...");
  }
  Serial.println("Koneksi WiFi berhasil");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT broker...");
    if (client.connect(clientID)) {
      Serial.println("terhubung");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println("Coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  // ESP_BT.begin("GAS_DECT"); //Name of your Bluetooth Signal
  // Serial.println("Bluetooth Device is Ready to Pair");
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // put your main code here, to run repeatedly:
  int mq7_data = analogRead(33);
  int mq135_data = analogRead(34);

  Serial.println(String(mq135_data).c_str());
  Serial.println(String(mq7_data).c_str());

  StaticJsonDocument<100> testDocument;
  char buffer[100];
  testDocument["mq7"] = mq7_data;
  testDocument["mq135"] = mq135_data;
  serializeJsonPretty(testDocument, buffer);
  
  client.publish("rafli/mq7135/gas/mq135",buffer);
  // testDocument = "";

  delay(2000);
}
