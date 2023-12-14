#ifndef PERFORMENCE_HPP
#define PERFORMENCE_HPP
#include <chrono>
class Performence {
public:
  ~Performence();
  // void init();
  void update();

  uint64_t get_fps() const;
  float get_delta() const;

private:
  uint64_t fps;
  float delta;
  std::chrono::steady_clock::time_point curr_time, last_time;
  std::vector<std::pair<uint64_t, float>> data_points;
};

#endif // !PERFORMENCE_HPP
