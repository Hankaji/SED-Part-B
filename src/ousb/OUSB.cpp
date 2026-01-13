#include "OUSB.h"
#include "../pipe/Pipe.h"
#include <iostream>
#include <sstream>
#include <string>

float OUSB::readPWM(int channel) {
#ifdef _WIN32
  std::string preCmd = "ousbMotorSim.exe -r pwm ";
#else
  std::string preCmd = "WINEDEBUG=-all  wine ousbMotorSim.exe -r pwm ";
#endif

  // Pipe::run("WINEDEBUG=-all  wine ousbMotorSim.exe -r pwm 1 50");
  std::string cmd = preCmd + std::to_string(channel);

  std::string output = Pipe::run(cmd);

  std::stringstream ss(output);
  float dutyPercent = -1.0f;
  ss >> dutyPercent;

  // Validate
  if (ss.fail() || dutyPercent < 0.0f || dutyPercent > 1.0f) {
    return -1.0f; // indicate failure
  }

  std::cout << "TEST " << dutyPercent << std::endl;

  return dutyPercent;
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
