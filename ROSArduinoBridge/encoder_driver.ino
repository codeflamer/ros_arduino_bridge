/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
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
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
    
   /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  void rotaryInterrupt() {
    static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t old_AB = 0b00000011; // 'rest' is HIGH (as it is pulled high)
  
    old_AB <<= 2;                                  // remember previous state
    old_AB |= (digitalRead(RIGHT_ENC_PIN_A) << 1) |   // add current state (2 bits)
               digitalRead(RIGHT_ENC_PIN_B);
    right_enc_pos += enc_states[old_AB & 0x0f];
  }

    /* Interrupt routine for LEFT encoder, taking care of actual counting */
  void rotaryInterruptLeft() {
    static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t old_AB = 0b00000011; // 'rest' is HIGH (as it is pulled high)
  
    old_AB <<= 2;                                  // remember previous state
    old_AB |= (digitalRead(LEFT_ENC_PIN_A) << 1) |   // add current state (2 bits)
               digitalRead(LEFT_ENC_PIN_B);
    left_enc_pos += enc_states[old_AB & 0x0f];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
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
