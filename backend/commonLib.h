#ifndef _COMMONLIB_H_
#define _COMMONLIB_H_

#define D(x) DEBUG(x)
#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x)                 \
  do {                           \
    std::cerr << x << std::endl; \
  } while (0)
#endif

#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
class commonLib {
 private:
 public:
  static std::string exec(std::string cmd) {
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL) result += buffer;
    }
    pclose(pipe);
    return result;
  }
  static std::vector<std::string> inline StringSplit(
      const std::string &source, const char *delimiter = " ",
      bool keepEmpty = false) {
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) !=
           std::string::npos) {
      if (keepEmpty || (next - prev != 0)) {
        results.push_back(source.substr(prev, next - prev));
      }
      prev = next + 1;
    }

    if (prev < source.size()) {
      results.push_back(source.substr(prev));
    }
    return results;
  }

  static int powInt(int a, int b) {
    if (b == 0) return 1;
    b--;
    int multiplier = a;
    while (b > 0) {
      b--;
      a = a * multiplier;
    }
    return a;
  }

  static unsigned int powUInt(unsigned int a, unsigned int b) {
    if (b == 0) return 1;
    b--;
    unsigned int multiplier = a;
    while (b > 0) {
      b--;
      a = a * multiplier;
    }
    return a;
  }

  static long powLong(long a, long b) {
    if (b == 0) return 1;
    b--;
    long multiplier = a;
    while (b > 0) {
      b--;
      a = a * multiplier;
    }
    return a;
  }

  static unsigned long powULong(unsigned long a, unsigned long b) {
    if (b == 0) return 1;
    b--;
    unsigned long multiplier = a;
    while (b > 0) {
      b--;
      a = a * multiplier;
    }
    return a;
  }
};
#endif