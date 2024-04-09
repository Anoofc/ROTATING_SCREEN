#define DEBUG 0
#define MOTOR_ID 254
#define GEAR_RATIO 4

#include <SCServo.h>

SCServo SERVO;

uint8_t rot_flag = 0;
uint16_t pos;
uint16_t currentMotorAngle;
uint16_t currentDiscAngle;
uint16_t prevDiscAngle;
uint16_t prevMotorAngle;
uint16_t servoPosition = 0;


void setup()
{
  Serial.begin(115200);
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
  currentMotorAngle = static_cast<float>(pos) / 4095 * 360 / GEAR_RATIO;

  if ((rot_flag == 0) && (prevDiscAngle == 89)  && (currentMotorAngle == 0)) {
    rot_flag = 1;
  } else if ((rot_flag == 1) && (prevDiscAngle == 179) && (currentMotorAngle == 0)) {
    rot_flag = 2;
  }
  else if ((rot_flag == 2) && (prevDiscAngle == 269) && (currentMotorAngle == 0)) {
    rot_flag = 3;
  } else if ((rot_flag == 3) && (prevDiscAngle == 359) && (currentMotorAngle == 0)) {
    rot_flag = 0;
  }


  if ((rot_flag == 0) && (prevDiscAngle == 0) && (currentMotorAngle == 89)) {
    rot_flag = 3;
  } else if ((rot_flag == 1) && (prevDiscAngle == 90) && (currentMotorAngle == 89)) {
    rot_flag = 0;
  } else if ((rot_flag == 2) && (prevDiscAngle == 180) && (currentMotorAngle == 89)) {
    rot_flag = 1;
  } else if ((rot_flag == 3) && (prevDiscAngle == 270) && (currentMotorAngle == 89)) {
    rot_flag = 2;
  }

  if (rot_flag == 0) {
    currentDiscAngle = currentMotorAngle;
  }
  else if (rot_flag == 1) {
    currentDiscAngle = currentMotorAngle + 90;
  }
  else if (rot_flag == 2) {
    currentDiscAngle = currentMotorAngle + 180;
  }
  else if (rot_flag == 3) {
    currentDiscAngle = currentMotorAngle + 270;
  }
  Serial.print("Disc Angle: ");
    Serial.println(currentDiscAngle);
  if (DEBUG) {
    Serial.print("Motor Position: ");
    Serial.print(pos);
    Serial.print("\t Motor Angle: ");
    Serial.print(currentMotorAngle);
    Serial.print("\t Disc Angle: ");
    Serial.println(currentDiscAngle);
  }
  prevDiscAngle = currentDiscAngle ;
  prevMotorAngle = currentMotorAngle;
}