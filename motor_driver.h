
#define RIGHT_MOTOR_FORWARD_ 11
#define RIGHT_MOTOR_BACKWARD 10
#define RIGHT_MOTOR_ENABLE__ 9

#define LEFT__MOTOR_FORWARD_ 7
#define LEFT__MOTOR_BACKWARD 8
#define LEFT__MOTOR_ENABLE__ 6

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
