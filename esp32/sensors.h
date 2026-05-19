#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "config.h"

static float    _pitch       = 0.0f;
static unsigned long _last_us = 0;
static int16_t  _ax, _ay, _az;
static int16_t  _gx, _gy, _gz;

static void imu_init() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(400000);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1C);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1B);
    Wire.write(0x08);
    Wire.endTransmission();

    delay(100);
    _last_us = micros();
}

static void imu_read() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)MPU_ADDR, (uint8_t)14, (uint8_t)true);

    _ax = (Wire.read() << 8) | Wire.read();
    _ay = (Wire.read() << 8) | Wire.read();
    _az = (Wire.read() << 8) | Wire.read();
    Wire.read(); Wire.read();
    _gx = (Wire.read() << 8) | Wire.read();
    _gy = (Wire.read() << 8) | Wire.read();
    _gz = (Wire.read() << 8) | Wire.read();
}

static float imu_pitch() {
    unsigned long now = micros();
    float dt = (now - _last_us) * 1e-6f;
    _last_us = now;

    float accel_pitch = atan2f((float)_ax, (float)_az) * 57.2957795f;
    float gyro_rate   = (float)_gy / 65.5f;

    _pitch = COMP_FILTER_ALPHA * (_pitch + gyro_rate * dt)
           + (1.0f - COMP_FILTER_ALPHA) * accel_pitch;

    return _pitch;
}

#endif // SENSORS_H
