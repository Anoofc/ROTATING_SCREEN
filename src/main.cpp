#define DEBUG 0

#define STEP 30       // CHANGE AS REQUIRED
#define MOTOR_ID 1
#define BAUDRATE 9600
#define GEAR_RATIO 4

#include "SCServo.h"

SCServo SERVO;

uint16_t pos;
uint16_t currentMotorAngle;
uint16_t prevMotorAngle;


void sendCMD() {

  if (currentMotorAngle % STEP == 0) {
    if (currentMotorAngle == 1 && prevMotorAngle == 360) { prevMotorAngle = 0; Serial.println("R");} // For correcting the direction of rotation
    if (currentMotorAngle == 360 && prevMotorAngle == 1) { prevMotorAngle = 360; Serial.println("L");}
    if (currentMotorAngle < prevMotorAngle) {
      Serial.println("L");
    } else if (currentMotorAngle > prevMotorAngle) {
      Serial.println("R");
    }
  }

}

void setup()
{
  Serial.begin(BAUDRATE);
  Serial2.begin(1000000);
  SERVO.pSerial = &Serial2;
  delay(500);
  SERVO.EnableTorque(1, 0);
  SERVO.EnableTorque(2, 1);
}

void loop()
{
  pos = SERVO.st_ReadPos(MOTOR_ID);
  // Calculate the actual angle of rotation for the disc
  currentMotorAngle = map (pos, 1, 4095, 1 ,360);

  if (DEBUG) {
    Serial.print("Motor Position: ");
    Serial.print(pos);
    Serial.print("\t Motor Angle: ");
    Serial.println(currentMotorAngle);
    
  }
  sendCMD();
  prevMotorAngle = currentMotorAngle;

}