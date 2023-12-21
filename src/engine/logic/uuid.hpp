#pragma once
#include <random>
namespace En {
class UUID {
public:
  bool operator==(const UUID &other) const { return this->id == other.id; }
  bool operator<(const UUID &other) const { return this->id < other.id; }

  UUID() { this->id = random() + random(); }
  std::string to_string() const { return std::to_string(this->id); }

private:
  uint64_t id;
};
} // namespace En
