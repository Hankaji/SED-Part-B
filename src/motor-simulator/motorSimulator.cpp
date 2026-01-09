#include "motorSimulator.h"
#include <algorithm>

MotorSimulator::MotorSimulator() {}
MotorSimulator::~MotorSimulator() {}

// PWN
float MotorSimulator::getPWM() { return this->PWM; }
void MotorSimulator::setPWM(float dutyCycle) {
  PWM = std::clamp(dutyCycle, 0.0f, 1.0f);

  // Simple simulation so ADC is only affected by PWN
  ADC = PWM * ADC_MAX;
}

// ADC
int MotorSimulator::getADC() { return this->ADC; }
