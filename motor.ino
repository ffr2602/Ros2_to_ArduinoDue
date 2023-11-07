void initMotorController() {
  digitalWrite(RIGHT_MOTOR_ENABLE__, HIGH);
  digitalWrite(LEFT__MOTOR_ENABLE__, HIGH);
}
void setMotorSpeed(int i, int spd) {
  unsigned char reverse = 0;
  if (spd < 0) {
    spd = -spd;
    reverse = 1;
  }
  if (spd > 90)
    spd = 90;
  if (i == LEFT) {
    if (reverse == 0) {
      analogWrite(LEFT__MOTOR_FORWARD_, spd);
      analogWrite(LEFT__MOTOR_BACKWARD, 0);
    } else if (reverse == 1) {
      analogWrite(LEFT__MOTOR_BACKWARD, spd);
      analogWrite(LEFT__MOTOR_FORWARD_, 0);
    }
  } else {
    if (reverse == 0) {
      analogWrite(RIGHT_MOTOR_FORWARD_, spd);
      analogWrite(RIGHT_MOTOR_BACKWARD, 0);
    } else if (reverse == 1) {
      analogWrite(RIGHT_MOTOR_BACKWARD, spd);
      analogWrite(RIGHT_MOTOR_FORWARD_, 0);
    }
  }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}
