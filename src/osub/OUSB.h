#pragma once

class OUSB {
public:
  bool setPWMDuty(int channel, int duty);
  int readADC(int channel);
  void disconnect();
};
