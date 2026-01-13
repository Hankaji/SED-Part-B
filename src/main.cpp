#include "ousb/OUSB.h"
#include "utils/time.h"
#include <algorithm>
#include <iostream>

int main(int argc, char *argv[]) {

  OUSB ousb;

  int targetADC = 500;
  int pwm = 40;

  const float Kp = 0.05f;  // Percentile to increase PWN, depends on error rate
                           // (Larger = more changes)
  const int tolerance = 5; // Acceptable error range
  const int MAX_LOOPS = 100;

  for (int i = 0; i < MAX_LOOPS; ++i) {
    int adc = ousb.readADC(0);
    if (adc < 0) {
      std::cerr << "[WARNING] Simulator disconnected\n";
      break;
    }

    int error = targetADC - adc;

    ousb.setPWMDuty(1, pwm);

    // Log to console
    std::cout << "[Loop " << i + 1 << "] PWM=" << pwm << "% ADC=" << adc
              << " Error=" << error << "\n";

    // Check if targetADC is within tolerance rate
    if (std::abs(error) <= tolerance) {
      std::cout << "Target reached.\n";
      break;
    }

    // Calculate rate of change and clamp it to a minimum of 1% change
    int delta = static_cast<int>(Kp * error);
    delta = std::min(delta, 1);

    pwm += delta;
    if (pwm < 0)
      pwm = 0;
    if (pwm > 100)
      pwm = 100;
  }
  return 0;
}
