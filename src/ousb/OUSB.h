#pragma once

class OUSB {
public:
  void setup();
  bool setPWMDuty(int channel, int duty);
  int readADC(int channel);
  void disconnect();
};
