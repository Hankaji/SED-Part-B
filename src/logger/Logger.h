#pragma once

#include <mutex>
#include <string>
#include <vector>

struct LogEntry {
  std::string time;
  int pwmPercent;
  int adc;
  int error;

  LogEntry(const std::string &t, int pwm, int a, int e);
};

class Logger {
private:
  std::vector<LogEntry> logs;
  std::mutex mutex;
  std::string logFilePath = "motor_log.csv";

  Logger();

  // Disable copy & assignment
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

public:
  static Logger &instance();
  ~Logger();

  void log(const std::string &time, int pwmPercent, int adc, int error);
  void clearLogs();
  void displayLog();
  void saveToFile();
  std::string getFilePath();
};
