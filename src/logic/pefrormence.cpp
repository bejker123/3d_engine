#include "performence.hpp"
#include <iostream>
#include <utility>

uint64_t counter = 0;
const double max_timer = 1000000000.0; // 1 s in nanoseconds
float timer = 0.0;

Performence::~Performence() {
  for (auto &i : data_points) {
    std::cout << "FPS: " << i.first
              << " timer: " << (double)i.second / max_timer << std::endl;
  }
}

void Performence::update() {
  this->curr_time = std::chrono::steady_clock::now();
  this->delta = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    this->curr_time - this->last_time)
                    .count();
  last_time = curr_time;
  if (delta < 0)
    delta = 0;
  if (timer >= max_timer) {
    fps = counter;
    this->data_points.push_back(std::make_pair(fps, timer));
    counter = 0;
    timer = 0;
  } else {
    timer += delta;
    counter++;
  }

  this->delta /= max_timer;
}

uint64_t Performence::get_fps() const { return this->fps; }
float Performence::get_delta() const { return this->delta; }
