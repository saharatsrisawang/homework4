#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Sensor and ThingSpeak settings
DHT dhtSensor(D4, DHT11);
const char* WRITE_KEY = "XZAII156UH1CI4AV";
const unsigned long CHANNEL_ID = 2412040;
const unsigned long TIME_DELAY = 15000;

// WiFi client for Internet connectivity
WiFiClient client;

// Initialize WiFi connection
void initializeWiFi(const String& ssid, const String& password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to " + ssid);
  Serial.println("IP address: " + WiFi.localIP().toString());
}

// Arduino setup function
void setup() {
  Serial.begin(115200);
  initializeWiFi("Xiaomi 11T Pro", "1212312126");
  dhtSensor.begin();
  ThingSpeak.begin(client);
}

// Read sensor data and send to ThingSpeak
void readAndSendSensorData() {
  float humidity = dhtSensor.readHumidity();
  float temperature = dhtSensor.readTemperature();
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);

  if (ThingSpeak.writeFields(CHANNEL_ID, WRITE_KEY) == 200) {
    Serial.println("Sent humidity and temperature to ThingSpeak");
  } else {
    Serial.println("Problem sending data to ThingSpeak");
  }
}

// Arduino main loop function
void loop() {
  readAndSendSensorData();
  delay(TIME_DELAY);
}