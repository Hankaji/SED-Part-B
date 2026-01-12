#include "TUI.h"

#include <iostream>
#include <limits>

#include "logger/logger.h"
#include "motor-simulator/motorSimulator.h"
#include "utils/time.h"

// Draw ASCII banner
static void drawHeader() {
  std::cout << "========================================\n"
               "   ELECTRIC MOTOR CONTROL SIMULATION\n"
               "========================================\n";
}

static int askTargetADC() {
  int target;

  while (true) {
    std::cout << "Enter target ADC (0–1023): ";
    std::cin >> target;

    if (!std::cin.fail() && target >= MotorSimulator::ADC_MIN &&
        target <= MotorSimulator::ADC_MAX) {
      return target;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Please try again.\n";
  }
}

// Closed-loop execution
static void runControlLoop(int targetADC) {
  MotorSimulator motor;
  Logger &logger = Logger::instance();

  float pwm = 0.45f;
  const float Kp = 0.0008f;
  const float minStep = 0.01f;
  const int tolerance = 5;
  const int MAX_LOOPS = 100;

  motor.setPWM(pwm);

  std::cout << "\nStarting feedback control...\n\n";

  for (int loop = 1; loop <= MAX_LOOPS; ++loop) {
    int adc = motor.getADC();
    if (adc < 0) {
      std::cerr << "[WARNING] Simulator communication failed.\n";
      break;
    }

    int error = targetADC - adc;

    motor.setPWM(pwm);

    std::string time = getCurrentTime();

    logger.log(time, static_cast<int>(pwm * 100), adc, error);

    std::cout << "[Loop " << loop << "] "
              << "PWM = " << static_cast<int>(pwm * 100) << "%, "
              << "ADC = " << adc << ", "
              << "Error = " << (error > 0 ? "+" : "") << error << "\n";

    if (std::abs(error) <= tolerance) {
      std::cout << "\nTarget reached! Final PWM = "
                << static_cast<int>(pwm * 100) << "%\n";
      return;
    }

    // Change PWN proportionally to error value, with at least 1% gain
    float deltaPWM = std::max(Kp * error, 0.01f);
    motor.setPWM(motor.getPWM() + deltaPWM);
    pwm = motor.getPWM();
  }

  std::cout << "\n[WARNING] Control loop terminated without convergence.\n";
}

void TUI::run() {
  drawHeader();

  int targetADC = askTargetADC();

  std::cout << "\nTarget ADC set to " << targetADC << "\n";

  runControlLoop(targetADC);

  Logger::instance().saveToFile(); // Just making sure
  std::cout << "Log saved to " << Logger::instance().getFilePath() << std::endl;
}
