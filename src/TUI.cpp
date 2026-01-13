#include "TUI.h"

#include <iostream>
#include <limits>

#include "logger/logger.h"
#include "motor-simulator/motorSimulator.h"
#include "ousb/OUSB.h"
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

  OUSB ousb;

  int pwm = 40;

  const float Kp = 0.05f;
  const int tolerance = 5;
  const int MAX_LOOPS = 100;

  for (int i = 0; i < MAX_LOOPS; ++i) {
    int adc = ousb.readADC(0);
    if (adc < 0) {
      std::cerr << "[WARNING] Simulator disconnected\n";
      break;
    }

    int error = targetADC - adc;

    int delta = static_cast<int>(Kp * error);
    if (delta == 0 && error != 0)
      delta = (error > 0) ? 1 : -1;

    pwm += delta;
    if (pwm < 0)
      pwm = 0;
    if (pwm > 100)
      pwm = 100;

    ousb.setPWMDuty(1, pwm);

    std::cout << "[Loop " << i + 1 << "] PWM=" << pwm << "% ADC=" << adc
              << " Error=" << error << "\n";

    if (std::abs(error) <= tolerance) {
      std::cout << "Target reached.\n";
      break;
    }
  }
}

void TUI::run() {
  drawHeader();

  int targetADC = askTargetADC();

  std::cout << "\nTarget ADC set to " << targetADC << "\n";

  runControlLoop(targetADC);

  Logger::instance().saveToFile(); // Just making sure
  std::cout << "Log saved to " << Logger::instance().getFilePath() << std::endl;
}
