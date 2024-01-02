#include "performence.hpp"
#include <iostream>

uint64_t counter = 0;
const double max_timer = 1000000000.0; // 1 s in nanoseconds
float timer = 0.0;
constexpr bool collect_data_points = false;

Performence::~Performence() {
  for (auto &i : data_points) {
    std::cout << "FPS: " << i.first
              << " timer: " << (double)i.second / max_timer << std::endl;
  }
}

void Performence::begin_update() {
  auto curr_time = std::chrono::steady_clock::now();
  this->delta = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    curr_time - this->last_time)
                    .count();
  last_time = curr_time;
  if (delta < 0)
    delta = 0;
  if (timer >= max_timer) {
    fps = counter;
    if (collect_data_points)
      this->data_points.push_back(std::make_pair(fps, timer));
    counter = 0;
    timer = 0;
  } else {
    timer += delta;
    counter++;
  }

  this->delta /= max_timer;
}

void Performence::end_update() {
  auto curr_time = std::chrono::steady_clock::now();
  this->cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                       curr_time - this->last_time)
                       .count();
  this->cpu_time /= max_timer;
}

void Performence::begin_render() {
  this->last_gpu_time = std::chrono::steady_clock::now();
}

void Performence::end_render() {
  auto curr_time = std::chrono::steady_clock::now();
  this->gpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                       curr_time - this->last_gpu_time)
                       .count();
  this->gpu_time /= max_timer;
}

void Performence::begin_vsync() {
  this->last_vsync_time = std::chrono::steady_clock::now();
}

void Performence::end_vsync() {
  auto curr_time = std::chrono::steady_clock::now();
  this->vsync_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                         curr_time - this->last_vsync_time)
                         .count();
  this->vsync_time /= max_timer;
}

uint64_t Performence::get_fps() const { return this->fps; }
float Performence::get_delta() const { return this->delta; }
float Performence::get_cpu_time() const { return this->cpu_time; }
float Performence::get_gpu_time() const { return this->gpu_time; }
float Performence::get_vsync_time() const { return this->vsync_time; }
