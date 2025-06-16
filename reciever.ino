#include <ESP8266WiFi.h>
#include <espnow.h>

const int ledOrderPin = D5;   // LED for "I want to give an order"
const int ledBillPin = D6;    // LED for "I want my bill"
const int ledServicePin = D7; // LED for "I want service"
const int buzzerPin = D8;     // Pin for the buzzer

void onDataRecv(uint8_t *macAddr, uint8_t *data, uint8_t len) {
  Serial.print("Received message: ");
  String receivedMessage = "";

  for (int i = 0; i < len; i++) {
    receivedMessage += (char)data[i];
  }
  Serial.println(receivedMessage);

  // Blink LED for "I want to give an order"
  if (receivedMessage == "I want to give an order") {
    digitalWrite(ledOrderPin, HIGH);
    tone(buzzerPin, 1000, 200); // Beep for "Order"
    delay(500);
    digitalWrite(ledOrderPin, LOW);
  }

  // Blink LED for "I want my bill"
  if (receivedMessage == "I want my bill") {
    digitalWrite(ledBillPin, HIGH);
    tone(buzzerPin, 1500, 200); // Beep for "Bill"
    delay(500);
    digitalWrite(ledBillPin, LOW);
  }

  // Blink LED for "I want service"
  if (receivedMessage == "I want service") {
    digitalWrite(ledServicePin, HIGH);
    tone(buzzerPin, 2000, 200); // Beep for "Service"
    delay(500);
    digitalWrite(ledServicePin, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  // Set LED and buzzer pins as outputs
  pinMode(ledOrderPin, OUTPUT);
  pinMode(ledBillPin, OUTPUT);
  pinMode(ledServicePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set the device as a slave
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  // Register the receive callback function
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("ESP-NOW Receiver Initialized");
}

void loop() {
  // No need to add code here; onDataRecv handles everything
}
