#include <ESP8266WiFi.h>
#include <espnow.h>

// Receiver MAC address
uint8_t receiverMAC[] = {0xE8, 0xDB, 0x84, 0xA6, 0xA8, 0xBE};

// Pin definitions
const int orderButtonPin = D1;  // Button for "I want to give an order"
const int billButtonPin = D2;   // Button for "I want my bill"
const int serviceButtonPin = D3; // Button for "I want service"
const int ledOrderPin = D5;     // LED for "I want to give an order"
const int ledBillPin = D6;      // LED for "I want my bill"
const int ledServicePin = D7;   // LED for "I want service"

// Variables
int orderButtonState = 0;
int billButtonState = 0;
int serviceButtonState = 0;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Set up the LED pins as outputs
  pinMode(ledOrderPin, OUTPUT);
  pinMode(ledBillPin, OUTPUT);
  pinMode(ledServicePin, OUTPUT);

  // Set up the button pins as inputs
  pinMode(orderButtonPin, INPUT_PULLUP);
  pinMode(billButtonPin, INPUT_PULLUP);
  pinMode(serviceButtonPin, INPUT_PULLUP);

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Add the receiver's MAC address
  if (esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER) != 0) {
    Serial.println("Failed to set device as controller");
    return;
  }
  if (esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 0, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read the button states
  orderButtonState = digitalRead(orderButtonPin);
  billButtonState = digitalRead(billButtonPin);
  serviceButtonState = digitalRead(serviceButtonPin);

  // If "I want to give an order" button is pressed
  if (orderButtonState == LOW) {
    Serial.println("I want to give an order");
    esp_now_send(receiverMAC, (uint8_t *)"I want to give an order", 23);
    digitalWrite(ledOrderPin, HIGH);
    delay(500);
    digitalWrite(ledOrderPin, LOW);
    delay(500);
  }

  // If "I want my bill" button is pressed
  if (billButtonState == LOW) {
    Serial.println("I want my bill");
    esp_now_send(receiverMAC, (uint8_t *)"I want my bill", 14);
    digitalWrite(ledBillPin, HIGH);
    delay(500);
    digitalWrite(ledBillPin, LOW);
    delay(500);
  }

  // If "I want service" button is pressed
  if (serviceButtonState == LOW) {
    Serial.println("I want service");
    esp_now_send(receiverMAC, (uint8_t *)"I want service", 15);
    digitalWrite(ledServicePin, HIGH);
    delay(500);
    digitalWrite(ledServicePin, LOW);
    delay(500);
  }

  delay(100); // Debounce delay
}
