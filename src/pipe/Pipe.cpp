#include "Pipe.h"
#include <array>
#include <cstdio>

std::string Pipe::run(const std::string &command) {
  std::array<char, 256> buffer{};
  std::string result;

  // Run cmd based one Window or Linux
#ifdef _WIN32
  FILE *pipe = _popen(command.c_str(), "r");
#else
  FILE *pipe = popen(command.c_str(), "r");
#endif

  if (!pipe)
    return "";

  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
    result += buffer.data();
  }

  // Run cmd based one Window or Linux
#ifdef _WIN32
  _pclose(pipe);
#else
  pclose(pipe);
#endif

  return result;
}
