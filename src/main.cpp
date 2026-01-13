#include "CLI.h"
#include "logger/logger.h"
#include "ousb/OUSB.h"
#include "utils/time.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void runClosedLoop(OUSB ousb, int targetADC) {
  Logger &logger = Logger::instance();

  int pwm = ousb.readPWM(1) * 100; // Map to 0-100 percent range

  const float Kp = 0.05f;  // Percentile to increase PWN, depends on error rate
                           // (Larger = more changes)
  const int tolerance = 5; // Acceptable error range
  const int MAX_LOOPS = 100;

  const int SATURATION_LIMIT = 5;
  int saturationCount = 0;

  bool reached = false;

  int i = 0;
  for (; i < MAX_LOOPS; ++i) {
    ousb.setPWMDuty(1, pwm);

    int adc = ousb.readADC(0);
    if (adc < 0) {
      std::cerr << "[ERROR] Simulator disconnected\n";
      break;
    }

    int error = targetADC - adc;

    // Log to console
    logger.log(getCurrentTime(), static_cast<int>(pwm * 100), adc, error);
    std::cout << "[Loop " << i + 1 << "] PWM=" << pwm << "% ADC=" << adc
              << " Error=" << error << "\n";

    // Check if targetADC is within tolerance rate
    if (std::abs(error) <= tolerance) {
      std::cout << "Target reached! Final PWM = " << pwm << "%" << std::endl;
      reached = true;
      break;
    }

    // Saturation detection
    if (pwm == 0 || pwm == 100) {
      saturationCount++;
      if (saturationCount >= SATURATION_LIMIT) {
        std::cerr << "[ERROR] PWM saturated at " << pwm << "% for "
                  << SATURATION_LIMIT << " consecutive cycles. "
                  << "Target ADC unreachable.\n";
        break;
      }
    } else {
      saturationCount = 0; // reset if PWM moves
    }

    // Calculate rate of change and clamp it to a minimum of 1% change
    int delta = static_cast<int>(Kp * error);
    delta = std::max(std::abs(delta), 1);
    // Copy sign (negative or positive) of error
    // Revert it because higher pwm = lower adc
    delta = -std::copysign(delta, error);

    pwm += delta;
    if (pwm < 0)
      pwm = 0;
    if (pwm > 100)
      pwm = 100;
  }

  if (!reached && i == MAX_LOOPS) {
    std::cerr
        << "[ERROR] Control loop terminated, max iterations loop reached\n";
  }

  std::cout << "\nLog saved to " << Logger::instance().getFilePath()
            << std::endl;
}

int main(int argc, char *argv[]) {

  CLIResult cmd = CLI::parse(argc, argv);

  OUSB ousb;

  switch (cmd.mode) {
  case CLIMode::PWM:
    ousb.setPWMDuty(1, cmd.value);
    std::cout << "PWM set to " << cmd.value << "%\n";
    break;

  case CLIMode::ADC: {
    int adc = ousb.readADC(cmd.value);
    std::cout << "ADC" << cmd.value << " = " << adc << "\n";
    break;
  }

  case CLIMode::LOOP:
    runClosedLoop(ousb, cmd.value); // your existing loop
    break;
  }

  return 0;
}
