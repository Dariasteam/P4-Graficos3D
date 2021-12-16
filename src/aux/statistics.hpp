#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <iostream>
#include <chrono>

class Statistics {
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> prev;
  unsigned ac {0};
public:
  Statistics() {
     prev = std::chrono::steady_clock::now();
  }

  inline void update () {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - prev;
    ac += 1;

    if (elapsed_seconds.count() >= 1.0) {
      std::cout << "Render FPS: " << ac << "s\n";
      ac = 0;
      prev = now;
    }
  }
};


#endif // _STATISTICS_H_

