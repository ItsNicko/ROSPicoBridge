/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSPicoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(PICO_GPIO_ENCODERS)
  volatile long left_enc_pos = 0;
  volatile long right_enc_pos = 0;
  volatile uint8_t left_enc_state = 0;
  volatile uint8_t right_enc_state = 0;
  static const int8_t ENC_STATES[] = {0, 1, -1, 0, -1, 0, 0, 1,
                                      1, 0, 0, -1, 0, -1, 1, 0};

  static inline uint8_t readLeftState() {
    return (digitalRead(LEFT_ENC_PIN_A) << 1) | digitalRead(LEFT_ENC_PIN_B);
  }

  static inline uint8_t readRightState() {
    return (digitalRead(RIGHT_ENC_PIN_A) << 1) | digitalRead(RIGHT_ENC_PIN_B);
  }

  void leftEncoderChanged() {
    uint8_t state = readLeftState();
    left_enc_pos += ENC_STATES[(left_enc_state << 2) | state];
    left_enc_state = state;
  }

  void rightEncoderChanged() {
    uint8_t state = readRightState();
    right_enc_pos += ENC_STATES[(right_enc_state << 2) | state];
    right_enc_state = state;
  }

  void initEncoders() {
    pinMode(LEFT_ENC_PIN_A, INPUT_PULLUP);
    pinMode(LEFT_ENC_PIN_B, INPUT_PULLUP);
    pinMode(RIGHT_ENC_PIN_A, INPUT_PULLUP);
    pinMode(RIGHT_ENC_PIN_B, INPUT_PULLUP);
    left_enc_state = readLeftState();
    right_enc_state = readRightState();
    attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN_A), leftEncoderChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN_B), leftEncoderChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_A), rightEncoderChanged, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN_B), rightEncoderChanged, CHANGE);
  }

  long readEncoder(int i) {
    noInterrupts();
    long count = (i == LEFT) ? left_enc_pos : right_enc_pos;
    interrupts();
    return count;
  }

  void resetEncoder(int i) {
    noInterrupts();
    if (i == LEFT) left_enc_pos = 0;
    else right_enc_pos = 0;
    interrupts();
  }
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif

