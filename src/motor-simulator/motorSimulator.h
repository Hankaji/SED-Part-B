#pragma once

#include <string>

class MotorSimulator {
private:
  float PWM = 0;
  int ADC = 0;

public:
  const int ADC_MIN = 0;
  const int ADC_MAX = 1023;

  MotorSimulator();
  ~MotorSimulator();

  float getPWM();
  // Set the value for PWN, value is clamped from range 0-1
  // (corresponding to 0% - 100%).
  // WARNING - Value will automatically be clamped if outside the range
  void setPWM(float dutyCycle);
  int getADC();

  long long getTimestamp() const;
};
