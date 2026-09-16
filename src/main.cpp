#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Audio.h"
#include "LittleFS.h"

// --- Configuration ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com"; // Replace with your MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic = "vernacular/alerts/#";

// --- I2S Pins for MAX98357A ---
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

WiFiClient espClient;
PubSubClient client(espClient);
Audio audio;

void setup() {
    Serial.begin(115200);
    
    // Initialize LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Setup Audio
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(15); // 0-21

    // Setup WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // Setup MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);
    reconnectMQTT();
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
    audio.loop(); // Required for I2S audio playback
}

void reconnectMQTT() {
    while (!client.connected()) {
        String clientId = "VoiceAlertNode-" + String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("MQTT Connected");
            client.subscribe(mqtt_topic);
        } else {
            Serial.print("MQTT Failed, rc=");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.printf("Received message on %s: %s\n", topic, message.c_str());

    // Parse JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.println("JSON Parse Failed");
        return;
    }

    const char* alertFile = doc["file"];
    if (alertFile) {
        String filePath = "/" + String(alertFile);
        Serial.printf("Playing audio: %s\n", filePath.c_str());
        
        // Play from LittleFS (Offline, fast, reliable)
        audio.connecttoFS(LittleFS, filePath.c_str());
        
        // Alternative: Stream directly from URL if online TTS is preferred
        // audio.connecttohost("http://your-server.com/audio/" + String(alertFile));
    }
}