#pragma once

enum class CLIMode { PWM, ADC, LOOP };

struct CLIResult {
  CLIMode mode;
  int value;
};

class CLI {
public:
  static CLIResult parse(int argc, char *argv[]);
};
