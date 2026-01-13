#include "CLI.h"
#include <cstdlib>
#include <iostream>
#include <string>

static void printUsage(const char *programName) {
  std::cerr << "Usage:\n"
            << "  " << programName << " P <0-100>   Set PWM duty\n"
            << "  " << programName
            << " A <0-7>     Read ADC channel (Only 0, 5, 6 supported for "
               "simulator)\n"
            << "  " << programName << " L <target>  Run feedback loop\n";
}

CLIResult CLI::parse(int argc, char *argv[]) {
  if (argc != 3) {
    printUsage(argv[0]);
    std::exit(1);
  }

  char modeChar = argv[1][0];
  int value;

  try {
    value = std::stoi(argv[2]);
  } catch (...) {
    std::cerr << "[ERROR] Value must be an integer\n";
    std::exit(1);
  }

  switch (modeChar) {
  case 'P': {
    if (value < 0 || value > 100) {
      std::cerr << "[ERROR] PWM duty must be 0–100\n";
      std::exit(1);
    }
    return {CLIMode::PWM, value};
  }

  case 'A': {
    if (value < 0 || value > 7) {
      std::cerr << "[ERROR] ADC channel must be 0–7\n";
      std::exit(1);
    }
    return {CLIMode::ADC, value};
  }

  case 'L': {
    if (value < 0 || value > 1023) {
      std::cerr << "[ERROR] Target ADC out of range\n";
      std::exit(1);
    }
    return {CLIMode::LOOP, value};
  }

  default:
    std::cerr << "[ERROR] Invalid mode. Use P, A, or L.\n";
    printUsage(argv[0]);
    std::exit(1);
  }
}
