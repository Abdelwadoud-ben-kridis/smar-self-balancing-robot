#ifndef MOTORS_H
#define MOTORS_H

#include "config.h"

static void motors_stop();

static void _motor_set(uint8_t in_a, uint8_t in_b, uint8_t ch, int16_t val) {
    if (val > 0) {
        digitalWrite(in_a, HIGH);
        digitalWrite(in_b, LOW);
        ledcWrite(ch, val);
    } else if (val < 0) {
        digitalWrite(in_a, LOW);
        digitalWrite(in_b, HIGH);
        ledcWrite(ch, -val);
    } else {
        digitalWrite(in_a, LOW);
        digitalWrite(in_b, LOW);
        ledcWrite(ch, 0);
    }
}

static void motors_init() {
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_IN3, OUTPUT);
    pinMode(PIN_IN4, OUTPUT);

    ledcSetup(MOTOR_PWM_CH_L, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcAttachPin(PIN_ENA, MOTOR_PWM_CH_L);

    ledcSetup(MOTOR_PWM_CH_R, MOTOR_PWM_FREQ, MOTOR_PWM_RES);
    ledcAttachPin(PIN_ENB, MOTOR_PWM_CH_R);

    motors_stop();
}

static void motors_drive(int16_t left, int16_t right) {
    left  = constrain(left,  -MOTOR_MAX_PWM, MOTOR_MAX_PWM);
    right = constrain(right, -MOTOR_MAX_PWM, MOTOR_MAX_PWM);

    _motor_set(PIN_IN1, PIN_IN2, MOTOR_PWM_CH_L, left);
    _motor_set(PIN_IN3, PIN_IN4, MOTOR_PWM_CH_R, right);
}

static void motors_balance(int16_t output) {
    motors_drive(output, output);
}

static void motors_stop() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    ledcWrite(MOTOR_PWM_CH_L, 0);
    ledcWrite(MOTOR_PWM_CH_R, 0);
}

#endif // MOTORS_H
