#pragma once

class OUSB {
public:
  float readPWM(int channel);
  bool setPWMDuty(int channel, int duty);
  int readADC(int channel);
  void disconnect();
};
