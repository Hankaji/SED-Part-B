#include "logger/logger.h"
#include "motor-simulator/motorSimulator.h"
#include "utils/time.h"
#include <algorithm>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3 || std::string(argv[1]) != "--targetADC") {
    std::cerr << "Usage: " << argv[0] << " --targetADC <value>\n";
    return 1;
  }

  MotorSimulator motor;

  int targetADC = 700;
  try {
    targetADC = std::stoi(argv[2]);
  } catch (...) {
    std::cerr << "Error: targetADC must be an integer\n";
    return 1;
  }

  const int tolerance = 3;
  const float step = 0.0008f;

  float pwm = 0.45f; // initial guess (45%)
  motor.setPWM(pwm);

  std::cout << "Target ADC = " << targetADC << "\n";
  std::cout << "Starting feedback control...\n";

  int loop = 1;

  while (true) {
    int adc = motor.getADC();
    int error = targetADC - adc;

    std::cout << "[Loop " << loop << "] "
              << "PWM = " << static_cast<int>(pwm * 100) << "%, "
              << "ADC = " << adc << ", "
              << "Error = " << (error > 0 ? "+" : "") << error << "\n";
    Logger::instance().log(getCurrentTime(), static_cast<int>(pwm * 100), adc,
                           error);

    if (std::abs(error) <= tolerance) {
      std::cout << "\nTarget reached! Final PWM = "
                << static_cast<int>(pwm * 100) << "%\n";
      break;
    }

    // Change PWN proportionally to error value, but never has lower than 1%
    // gain
    float deltaPWM = std::max(step * error, 0.01f);
    motor.setPWM(motor.getPWM() + deltaPWM);
    pwm = motor.getPWM();

    loop++;
  }

  std::cout << "Log saved to " << Logger::instance().getFilePath() << std::endl;

  return 0;
}
