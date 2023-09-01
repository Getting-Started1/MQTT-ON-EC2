#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = ""; //change this
const char* password = ""; //change this

// Replace with your MQTT broker information
const char* mqtt_server = ""; //change this
const int mqtt_port = 1883;
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic_1 = "sensor_data_1";
const char* mqtt_topic_2 = "sensor_data_2";
const char* mqtt_topic_3 = "sensor_data_3";

// Initialize the DHT sensor
#define DHTPIN 7 // Define the pin for DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Initialize WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      //Subscribe to addtition MQTT topics here
      client.subscribe(mqtt_topic_1);
      client.subscribe(mqtt_topic_2);
      client.subscribe(mqtt_topic_3);


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  String payload = String(temperature) + "," + String(humidity);
  client.publish(mqtt_topic_1, payload.c_str());

  delay(5000); // Publish every 5 seconds
}
