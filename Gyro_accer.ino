
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
const int ledPin = 5;  // Assuming the LED is connected to digital pin 13
const int buzzerPin = 12;  // Assuming the buzzer is connected to digital pin 9

const int buttonPin = 34;
//unsigned long alertStartTime = 0;
bool alertActive = false;

Adafruit_MPU6050 mpu;

void activateAlert() {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);

  delay(3000);

  digitalWrite(ledPin, LOW);
  analogWrite(buzzerPin, 128);
  alertActive = false;
}

//void deactivateAlert() {
//  digitalWrite(ledPin, LOW);
//  digitalWrite(buzzerPin, LOW);
//}

void checkFallConditions(const sensors_event_t& accel, const sensors_event_t& gyro, bool buttonState) {
  // ... (existing fall detection code)
  float accelThreshold = -10.0; // Adjust as needed based on testing
  float gyroThreshold = 1.15; // Adjust as needed based on testing

  if (buttonState == HIGH) {
   Serial.println("******************Alert: Emergency Button Presesed************************");
   activateAlert();
   return;
  }

  // Check for both accelerometer Z-axis and gyroscope conditions
  if (accel.acceleration.z < -accelThreshold && (gyro.gyro.x > gyroThreshold || gyro.gyro.y > gyroThreshold || gyro.gyro.z > gyroThreshold)) {
    Serial.println("******************Alert: Fall Detected - Accelerometer Z-axis AND Gyroscope************************");
    if (!alertActive) {
//      alertActive = true;
//      alertStartTime = millis();
      activateAlert();
    }
    // Notify user or emergency services
    // Example: sendSMS("Fall detected, user needs help");
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(1000);
}


void loop() {
  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  bool buttonState = digitalRead(buttonPin);

  // Print accelerometer and gyroscope values to Serial Monitor
  Serial.print("Acceleration: ");
  Serial.print(a.acceleration.x);
  Serial.print(", ");
  Serial.print(a.acceleration.y);
  Serial.print(", ");
  Serial.print(a.acceleration.z);
  Serial.print(" m/s^2\t");

  Serial.print("Rotation: ");
  Serial.print(g.gyro.x);
  Serial.print(", ");
  Serial.print(g.gyro.y);
  Serial.print(", ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  // Check for fall conditions
  checkFallConditions(a, g, buttonState);
  
  delay(500);
}
