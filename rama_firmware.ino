

#define BAUDRATE 57600
#define MAX_PWM 255
#define PID_RATE 30
#define AUTO_STOP_INTERVAL 2000

#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "diff_controller.h"

const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;
long lastMotorCommand = AUTO_STOP_INTERVAL;

int arg = 0;
int indesk = 0;

char chr;
char cmd;

char argv1[16];
char argv2[16];

long arg1;
long arg2;

void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  indesk = 0;
}

int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch (cmd) {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;
    case READ_ENCODERS:
      Serial.print(readEncoder(LEFT));
      Serial.print(" ");
      Serial.println(readEncoder(RIGHT));
      break;
    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0) {
        setMotorSpeeds(0, 0);
        resetPID();
        moving = 0;
      } else moving = 1;
      leftPID.TargetTicksPerFrame = arg1;
      rightPID.TargetTicksPerFrame = arg2;
      Serial.println("OK");
      break;
    case MOTOR_RAW_PWM:
      lastMotorCommand = millis();
      resetPID();
      moving = 0;
      setMotorSpeeds(arg1, arg2);
      Serial.println("OK");
      break;
    case UPDATE_PID:
      while ((str = strtok_r(p, ":", &p)) != '\0') {
        pid_args[i] = atoi(str);
        i++;
      }
      Kp = pid_args[0];
      Kd = pid_args[1];
      Ki = pid_args[2];
      Ko = pid_args[3];
      Serial.println("OK");
      break;
    default:
      Serial.println("Invalid Command");
      break;
  }
}

void setup() {
  Serial.begin(BAUDRATE);
  pinMode(LEFT__MOTOR_ENABLE__, OUTPUT);
  pinMode(RIGHT_MOTOR_ENABLE__, OUTPUT);
  pinMode(LEFT__MOTOR_FORWARD_, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD_, OUTPUT);
  pinMode(LEFT__MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_ENC_PIN_A_, INPUT);
  pinMode(LEFT_ENC_PIN_B_, INPUT);
  pinMode(RIGHT_ENC_PIN_A, INPUT);
  pinMode(RIGHT_ENC_PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN_A_), updateEncoder_left_, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN_B_), updateEncoder_left_, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_A), updateEncoder_right, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_B), updateEncoder_right, CHANGE);
  initMotorController();
  resetPID();
}

void loop() {
  while (Serial.available() > 0) {
    chr = Serial.read();
    if (chr == 13) {
      if (arg == 1) argv1[indesk] = NULL;
      else if (arg == 2) argv2[indesk] = NULL;
      runCommand();
      resetCommand();
    } else if (chr == ' ') {
      if (arg == 0) arg = 1;
      else if (arg == 1) {
        argv1[indesk] = NULL;
        arg = 2;
        indesk = 0;
      }
      continue;
    } else {
      if (arg == 0) {
        cmd = chr;
      } else if (arg == 1) {
        argv1[indesk] = chr;
        indesk++;
      } else if (arg == 2) {
        argv2[indesk] = chr;
        indesk++;
      }
    }
  }
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    ;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}
