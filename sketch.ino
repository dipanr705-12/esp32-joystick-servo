#include <ESP32Servo.h>

const int JOY_X_PIN = 34;   // Joystick horizontal axis
const int SERVO_PIN  = 18;  // 360° continuous servo signal

Servo myServo;

// Deadzone around center to stop the servo (joystick drift fix)
const int DEADZONE = 100;

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);

  // Stop the servo at start
  myServo.write(90);

  Serial.println("360 Degree Continuous Servo Ready!");
}

void loop() {
  int joyVal = analogRead(JOY_X_PIN);  // 0 to 4095

  // Map joystick to 0-180 range (90 = stop for 360° servo)
  int speed = map(joyVal, 0, 4095, 0, 180);

  // Apply deadzone around center (90) to prevent drift
  if (abs(speed - 90) < DEADZONE / 20) {
    speed = 90;  // Stop
  }

  myServo.write(speed);

  String dir;
  if      (speed < 85)  dir = "Clockwise";
  else if (speed > 95)  dir = "Counter-Clockwise";
  else                  dir = "STOPPED";

  Serial.print("Joy: ");
  Serial.print(joyVal);
  Serial.print(" | Speed cmd: ");
  Serial.print(speed);
  Serial.print(" | ");
  Serial.println(dir);

  delay(20);
}
