#pragma once
#include <chrono>
class Performence {
public:
  ~Performence();
  // void init();
  void update();
  void begin_update();
  void end_update();
  void begin_render();
  void end_render();
  void begin_vsync();
  void end_vsync();

  uint64_t get_fps() const;
  float get_delta() const;
  float get_cpu_time() const;
  float get_gpu_time() const;
  float get_vsync_time() const;

private:
  uint64_t fps;
  float delta, cpu_time, gpu_time, vsync_time;
  std::chrono::steady_clock::time_point last_time, last_gpu_time,
      last_vsync_time;
  std::vector<std::pair<uint64_t, float>> data_points;
};
