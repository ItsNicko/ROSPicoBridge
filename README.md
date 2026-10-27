# Raspberry Pi Pico ROS Motor Bridge

Firmware for a Raspberry Pi Pico (RP2040) that drives two motors through an L298N, reads two quadrature encoders, and communicates with a ROS host over USB serial. ROS runs on a separate host; this repository contains the Pico firmware, not the ROS node.

## Attribution

This project is adapted from Josh Newans' [ros_arduino_bridge](https://github.com/joshnewans/ros_arduino_bridge), itself a fork of the HBRC ROS Arduino Bridge. This repository adds the Raspberry Pi Pico GPIO encoder and PWM configuration. The original copyright and BSD license notice is retained in the main sketch.

## Build and upload

1. Install the Raspberry Pi Pico/RP2040 board package using the [Arduino-Pico installation guide](https://arduino-pico.readthedocs.io/en/latest/install.html).
2. Open `ROSPicoBridge/ROSPicoBridge.ino` in Arduino IDE.
3. Select **Raspberry Pi Pico** as the board, select its port, then compile and upload.

The sketch uses Earle Philhower's Arduino-Pico core. The default configuration enables the L298N driver and GPIO quadrature encoders. Servo support is disabled.

## Default wiring

The values below are Pico GPIO numbers, not physical header pin numbers. Edit `motor_driver.h` or `encoder_driver.h` to change them.

| Signal                         | Pico GPIO   |
| ------------------------------ | ----------- |
| Left encoder A / B             | GP2 / GP3   |
| Right encoder A / B            | GP4 / GP7   |
| Left motor forward / backward  | GP10 / GP6  |
| Right motor forward / backward | GP9 / GP5   |
| L298N left / right enable      | GP13 / GP12 |

Connect Pico ground, encoder ground, and L298N logic ground together. Power the motors from a suitable external motor supply, not from the Pico. Pico GPIO is 3.3 V and is not 5 V tolerant; encoder outputs must stay within that range. Remove the L298N enable jumpers when using GP12 and GP13 to control the enable inputs.

## Serial protocol

Use the Pico USB serial port at 57600 baud. Send one command per line, terminated with a carriage return (`CR`). The protocol is the same single-character command protocol used by the upstream bridge.

| Command | Arguments        | Action                                                                |
| ------- | ---------------- | --------------------------------------------------------------------- |
| `b`     | (none)           | Return the configured baud rate                                       |
| `e`     | (none)           | Return left and right encoder counts                                  |
| `r`     | (none)           | Reset both encoder counts and PID state                               |
| `m`     | `left right`     | Set closed-loop target counts per PID frame                           |
| `o`     | `left right`     | Set raw motor PWM from -255 to 255                                    |
| `u`     | `Kp:Kd:Ki:Ko`    | Update PID gains                                                      |
| `p`     | `pin`            | Read a PING-style ultrasonic sensor on the pin; return distance in cm |
| `a`     | `pin`            | Read an analog pin                                                    |
| `c`     | `pin mode`       | Set pin mode: 0 for input, 1 for output                               |
| `d`     | `pin`            | Read a digital pin                                                    |
| `w`     | `pin value`      | Write a digital pin: 0 or 1                                           |
| `x`     | `pin value`      | Write an analog/PWM value                                             |
| `s`     | `index position` | Set a servo target; unavailable while servo support is disabled       |
| `t`     | `index`          | Read a servo position; unavailable while servo support is disabled    |

For example, reset the encoders with `r\r`, set raw PWM with `o 80 -80\r`, or set the PID gains with `u 20:12:0:50\r`. The firmware stops the motors if it receives no movement command for two seconds, so a host using motor control must keep sending commands.

The closed-loop motor speeds are encoder counts per PID frame. The PID loop runs at 30 Hz by default, so convert counts per second to counts per frame by dividing by 30.
