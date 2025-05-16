#include <Wire.h>
#include <RTClib.h>

// DS3231 RTC
RTC_DS3231 rtc;

// Pins
const int relay1Pin = 2; // Relay for Motor 1
const int relay2Pin = 3; // Relay for Motor 2
const int buttonPin = 4; // Button input
const int piezoPin = 5;  // Piezo buzzer

// Variables
bool alarmActive = false; // Tracks if the alarm is active
bool buttonPressed = false; // Tracks if the button is pressed

// Alarm time
const int alarmHour = 7;  // Set alarm hour (24-hour format)
const int alarmMinute = 0; // Set alarm minute

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Setup pins
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(piezoPin, OUTPUT);

  // Turn off motors initially
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Get current time from RTC
  DateTime now = rtc.now();

  // Check if the alarm time is reached
  if (now.hour() == alarmHour && now.minute() == alarmMinute && !alarmActive) {
    alarmActive = true; // Activate alarm
    Serial.println("Alarm Activated!");
  }

  // If the alarm is active, run motors and sound buzzer
  if (alarmActive) {
    runMotors();
    soundAlarm();

    // Check if the button is pressed to stop the alarm
    if (digitalRead(buttonPin) == LOW) {
      stopAlarm();
    }
  } else {
    stopMotors(); // Ensure motors are off when the alarm is not active
  }

  delay(100); // Short delay for stability
}

void runMotors() {
  // Turn on relays to activate motors
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  Serial.println("Motors ON - Alarm driving away!");
}

void stopMotors() {
  // Turn off relays to deactivate motors
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  Serial.println("Motors OFF");
}

void soundAlarm() {
  // Continuously sound the buzzer
  tone(piezoPin, 1000); // 1kHz tone
  Serial.println("Buzzer ON - Wake up!");
}

void stopAlarm() {
  // Deactivate alarm
  alarmActive = false;

  // Turn off motors and buzzer
  stopMotors();
  noTone(piezoPin);
  Serial.println("Alarm Stopped - Button Pressed");
}