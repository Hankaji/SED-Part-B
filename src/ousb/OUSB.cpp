#include "OUSB.h"
#include "../pipe/Pipe.h"
#include <sstream>
#include <string>

void OUSB::setup() {
#ifdef _WIN32
  Pipe::run("ousbMotorSim.exe -r pwm-freq 1 46");
#else
  Pipe::run("WINEDEBUG=-all  wine ousbMotorSim.exe -r pwm-freq 1 46");
#endif
}

bool OUSB::setPWMDuty(int channel, int duty) {
  if (duty < 0 || duty > 100)
    return false;

#ifdef _WIN32
  std::string preCmd = "ousbMotorSim.exe -r pwm ";
#else
  std::string preCmd = "WINEDEBUG=-all  wine ousbMotorSim.exe -r pwm ";
#endif

  std::string cmd =
      preCmd + std::to_string(channel) + " " + std::to_string(duty);

  Pipe::run(cmd);
  return true;
}

int OUSB::readADC(int channel) {
#ifdef _WIN32
  std::string preCmd = "ousbMotorSim.exe -r adc ";
#else
  std::string preCmd = "WINEDEBUG=-all  wine ousbMotorSim.exe -r adc ";
#endif

  std::string cmd = preCmd + std::to_string(channel);

  std::string output = Pipe::run(cmd);

  std::stringstream ss(output);
  int value;
  ss >> value;

  if (ss.fail())
    return -1; // communication failure
  return value;
}

void OUSB::disconnect() { Pipe::run("ousbMotorSim.exe -disconnect"); }
