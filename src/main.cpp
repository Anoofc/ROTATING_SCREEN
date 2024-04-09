#define DEBUG 0

#define STEP        10 // CHANGE AS REQUIRED
#define MOTOR_ID    254
#define BAUDRATE    9600
#define GEAR_RATIO  4

#include <SCServo.h>
#include "Arduino.h"

SCServo SERVO;

uint8_t rot_flag = 0;
uint16_t pos;
uint16_t currentMotorAngle;
uint16_t currentDiscAngle;
uint16_t prevDiscAngle;
uint16_t prevMotorAngle;
uint16_t prevAngle = 0;

void sendCMD()
{
  if (currentDiscAngle % STEP == 0)
  {
    if      (currentDiscAngle < prevDiscAngle) { Serial.println("L"); }
    else if (currentDiscAngle > prevDiscAngle) { Serial.println("R"); }
  }
}


void read_position()
{
  pos = SERVO.st_ReadPos(MOTOR_ID);
  // Calculate the actual angle of rotation for the disc
  currentMotorAngle = static_cast<float>(pos) / 4095 * 360 / GEAR_RATIO;
}
void update_motor_angle()
{
  if      (rot_flag == 0) { currentDiscAngle = currentMotorAngle; }
  else if (rot_flag == 1) { currentDiscAngle = currentMotorAngle + 90; }
  else if (rot_flag == 2) { currentDiscAngle = currentMotorAngle + 180; }
  else if (rot_flag == 3) { currentDiscAngle = currentMotorAngle + 270; }
}
void update_rot_flag()
{
  if      ((rot_flag == 0) && (prevDiscAngle == 89)  && (currentMotorAngle == 0)) { rot_flag = 1; }
  else if ((rot_flag == 1) && (prevDiscAngle == 179) && (currentMotorAngle == 0)) { rot_flag = 2; }
  else if ((rot_flag == 2) && (prevDiscAngle == 269) && (currentMotorAngle == 0)) { rot_flag = 3; }
  else if ((rot_flag == 3) && (prevDiscAngle == 359) && (currentMotorAngle == 0)) { rot_flag = 0; }

  if ((rot_flag == 0) && (prevDiscAngle == 0)   && (currentMotorAngle == 89)) { rot_flag = 3; }
  if ((rot_flag == 1) && (prevDiscAngle == 90)  && (currentMotorAngle == 89)) { rot_flag = 0; }
  if ((rot_flag == 2) && (prevDiscAngle == 180) && (currentMotorAngle == 89)) { rot_flag = 1; }
  if ((rot_flag == 3) && (prevDiscAngle == 270) && (currentMotorAngle == 89)) { rot_flag = 2; }
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
  read_position();
  update_rot_flag();
  update_motor_angle();
  sendCMD();

  if (DEBUG)
  {
    Serial.println("Motor Position: " + String(pos) + "\t Motor Angle: " + String(currentMotorAngle) + "\t FLAG: " + String(rot_flag) + "\t Disc Angle: " + String(currentDiscAngle));
  }

  prevDiscAngle = currentDiscAngle;
  prevMotorAngle = currentMotorAngle;
}
