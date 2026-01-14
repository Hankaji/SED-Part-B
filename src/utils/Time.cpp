#include "Time.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

// Get current time in HH:MM:SS format
std::string getCurrentTime() {
  using namespace std::chrono;

  auto now = system_clock::now();
  std::time_t now_c = system_clock::to_time_t(now);

  std::tm localTime{};
#ifdef _WIN32
  localtime_s(&localTime, &now_c);
#else
  localtime_r(&now_c, &localTime);
#endif

  std::ostringstream oss;
  oss << std::put_time(&localTime, "%H:%M:%S");
  return oss.str();
}
