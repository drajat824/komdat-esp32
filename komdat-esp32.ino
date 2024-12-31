#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

// UUID unik untuk service dan characteristic BLE
#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

// MOTOR
#define MRightCycle 15
#define MRight1 2
#define MRight2 4

#define MLeft1 18
#define MLeft2 19
#define MLeftCycle 21
int dutyCycle = 255;

// Setting PWM properties
const int freq = 30000;
const int pwmChannelLeft = 0;
const int pwmChannelRight = 1;
const int resolution = 8;

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32_RC");

  pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setValue("Ready");
  pService->start();
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("BLE device is ready to connect...");

  // Motor Pin Configuration
  pinMode(MLeft1, OUTPUT);
  pinMode(MLeft2, OUTPUT);
  pinMode(MLeftCycle, OUTPUT);

  pinMode(MRight1, OUTPUT);
  pinMode(MRight2, OUTPUT);
  pinMode(MRightCycle, OUTPUT);

  pinMode(12, OUTPUT);

  Serial.println("Setup motor success");
  delay(100);
}

void loop() {
  String value = pCharacteristic->getValue();
  Serial.println(value);

  int ldrValue = analogRead(13);
  Serial.println(ldrValue);
  if (ldrValue > 400) {
    digitalWrite(12, LOW);
  } else {
    digitalWrite(12, HIGH);
  }

  // if (!value.isEmpty() && value != "Ready") {
  if (value == "F") {
    Serial.println("Moving Forward");
    moveForward();
  } else if (value == "B") {
    Serial.println("Moving Backward");
    moveBackward();
  } else if (value == "L") {
    Serial.println("Moving LEFT");
    moveLeft();
  } else if (value == "R") {
    Serial.println("Moving RIGHT");
    moveRight();
  } else {
    Serial.println("Moving STOP");
    moveStop();
  }
  pCharacteristic->setValue("Ready");
  // }

  delay(300);  // Waktu tunggu sebelum membaca lagi
}

void moveForward() {
  digitalWrite(MLeft1, HIGH);
  digitalWrite(MLeft2, LOW);
  digitalWrite(MRight1, HIGH);
  digitalWrite(MRight2, LOW);

  analogWrite(MLeftCycle, 255);
  analogWrite(MRightCycle, 255);
}

void moveBackward() {
  digitalWrite(MLeft1, LOW);
  digitalWrite(MLeft2, HIGH);
  digitalWrite(MRight1, LOW);
  digitalWrite(MRight2, HIGH);

  analogWrite(MLeftCycle, 255);
  analogWrite(MRightCycle, 255);
}

void moveStop() {
  digitalWrite(MLeft1, LOW);
  digitalWrite(MLeft2, LOW);
  digitalWrite(MRight1, LOW);
  digitalWrite(MRight2, LOW);
}

void moveLeft() {
  digitalWrite(MLeft1, HIGH);
  digitalWrite(MLeft2, LOW);
  digitalWrite(MRight1, LOW);
  digitalWrite(MRight2, LOW);

  analogWrite(MLeftCycle, 255);
  analogWrite(MRightCycle, 255);
}

void moveRight() {
  digitalWrite(MLeft1, LOW);
  digitalWrite(MLeft2, LOW);
  digitalWrite(MRight1, HIGH);
  digitalWrite(MRight2, LOW);

  analogWrite(MLeftCycle, 255);
  analogWrite(MRightCycle, 255);
}