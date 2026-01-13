#include "../pipe/Pipe.h"
#include "OUSB.h"
#include <sstream>
#include <string>

bool OUSB::setPWMDuty(int channel, int duty) {
  if (duty < 0 || duty > 100)
    return false;

  std::string cmd = "ousbMotorSim.exe -r pwm " + std::to_string(channel) + " " +
                    std::to_string(duty);

  Pipe::run(cmd);
  return true;
}

int OUSB::readADC(int channel) {
  std::string cmd = "ousbMotorSim.exe -r adc " + std::to_string(channel);

  std::string output = Pipe::run(cmd);

  std::stringstream ss(output);
  int value;
  ss >> value;

  if (ss.fail())
    return -1; // communication failure
  return value;
}

void OUSB::disconnect() { Pipe::run("ousbMotorSim.exe -disconnect"); }
