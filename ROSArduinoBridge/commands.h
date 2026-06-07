/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#define ANALOG_READ    'a'
#define GET_BAUDRATE   'b'
#define PIN_MODE       'c'
#define DIGITAL_READ   'd'
#define READ_ENCODERS  'e'
#define MOTOR_SPEEDS   'm'
#define MOTOR_RAW_PWM  'o'
#define PING           'p'
#define RESET_ENCODERS 'r'
#define SERVO_WRITE    's'
#define SERVO_READ     't'
#define UPDATE_PID     'u'
#define DIGITAL_WRITE  'w'
#define ANALOG_WRITE   'x'
#define LEFT            0
#define RIGHT           1

/* Pump control commands */
#define PUMP1_ON        'P'  // Turn pump 1 on
#define PUMP1_OFF       'Q'  // Turn pump 1 off  
#define PUMP2_ON        'R'  // Turn pump 2 on
#define PUMP2_OFF       'S'  // Turn pump 2 off
#define PUMP_SET_DURATION 'T' // Set pump duration (format: T <pump_num> <duration_ms>)
#define PUMP_BOTH_OFF   'U'  // Turn both pumps off
#define PUMP_STATUS     'V'  // Get pump status

#endif  // <-- THIS WAS MISSING - MAKE SURE THIS LINE EXISTS!