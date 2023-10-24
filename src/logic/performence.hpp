#ifndef PERFORMENCE_HPP
#define PERFORMENCE_HPP
#include <chrono>
#include <cstdint>
class Performence {
public:
  // void init();
  void update();

  uint64_t get_fps() const;
  float get_delta() const;

private:
  uint64_t fps;
  float delta;
  std::chrono::steady_clock::time_point curr_time, last_time;
};

#endif // !PERFORMENCE_HPP
