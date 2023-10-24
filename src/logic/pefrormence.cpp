#include "performence.hpp"

uint64_t counter = 0;
const double max_timer = 1000000000.0;
float timer = 0.0;

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
