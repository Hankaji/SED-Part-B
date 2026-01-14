#include "Logger.h"
#include <fstream>
#include <iostream>

LogEntry::LogEntry(const std::string &t, int pwm, int a, int e)
    : time(t), pwmPercent(pwm), adc(a), error(e) {}

Logger::Logger() {}

Logger::~Logger() { saveToFile(); }

Logger &Logger::instance() {
  static Logger instance;
  return instance;
}

void Logger::log(const std::string &time, int pwmPercent, int adc, int error) {
  std::lock_guard<std::mutex> lock(mutex);
  logs.emplace_back(time, pwmPercent, adc, error);
}

void Logger::clearLogs() {
  std::lock_guard<std::mutex> lock(mutex);
  logs.clear();
}

void Logger::displayLog() {
  std::lock_guard<std::mutex> lock(mutex);

  for (const auto &e : logs) {
    std::cout << "[" << e.time << "] "
              << "PWM=" << e.pwmPercent << "%, "
              << "ADC=" << e.adc << ", "
              << "Error=" << e.error << std::endl;
  }
}

std::string Logger::getFilePath() { return logFilePath; }

void Logger::saveToFile() {

  std::lock_guard<std::mutex> lock(mutex);

  std::ofstream out(logFilePath, std::ios::trunc);
  if (!out)
    return;

  // CSV header
  out << "Time,PWM(%),ADC,Error\n";

  for (const auto &e : logs) {
    out << e.time << "," << e.pwmPercent << "," << e.adc << "," << e.error
        << "\n";
  }
}
