volatile long left_enc_pos_ = 0L;
volatile long right_enc_pos = 0L;
volatile int encoderDirection__left = 0;
volatile int encoderDirection_right = 0;

void updateEncoder_left_() {
  static byte encoderA, encoderB;
  byte encoderNewA = digitalRead(LEFT_ENC_PIN_A_);
  byte encoderNewB = digitalRead(LEFT_ENC_PIN_B_);

  int change = 0;
  if (encoderA == LOW && encoderNewA == HIGH) {
    change = (encoderB == LOW) ? 1 : -1;
  } else if (encoderA == HIGH && encoderNewA == LOW) {
    change = (encoderB == HIGH) ? 1 : -1;
  }

  encoderDirection__left = change;
  left_enc_pos_ += encoderDirection__left;

  encoderA = encoderNewA;
  encoderB = encoderNewB;
}

void updateEncoder_right() {
  static byte encoderA, encoderB;
  byte encoderNewA = digitalRead(RIGHT_ENC_PIN_A);
  byte encoderNewB = digitalRead(RIGHT_ENC_PIN_B);

  int change = 0;
  if (encoderA == LOW && encoderNewA == HIGH) {
    change = (encoderB == HIGH) ? 1 : -1;
  } else if (encoderA == HIGH && encoderNewA == LOW) {
    change = (encoderB == LOW) ? 1 : -1;
  }

  encoderDirection_right = change;
  right_enc_pos += encoderDirection_right;

  encoderA = encoderNewA;
  encoderB = encoderNewB;
}

long readEncoder(int i) {
  if (i == LEFT) return left_enc_pos_;
  else return right_enc_pos;
}

void resetEncoder(int i) {
  if (i == LEFT) {
    left_enc_pos_ = 0L;
    return;
  } else {
    right_enc_pos = 0L;
    return;
  }
}

void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}
