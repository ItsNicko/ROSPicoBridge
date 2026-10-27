/* *************************************************************
 *Encoder driver function definitions - by James Nugen*
 ************************************************************ */

#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

#ifdef PICO_GPIO_ENCODERS
// Raspberry Pi Pico GPIO numbers; change to match the wiring.
#define LEFT_ENC_PIN_A 2
#define LEFT_ENC_PIN_B 3
#define RIGHT_ENC_PIN_A 4
#define RIGHT_ENC_PIN_B 7
#endif

long readEncoder(int i);
void initEncoders();
void resetEncoder(int i);
void resetEncoders();

#endif // ENCODER_DRIVER_H
