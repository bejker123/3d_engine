#ifndef BATCH_HPP
#define BATCH_HPP

#include "model.hpp"
#include <memory>
#include <vector>
class Batch {
public:
  static std::shared_ptr<Batch> instance() {
    static std::shared_ptr<Batch> b(new Batch);
    return b;
  }

  void render() {}

private:
  Batch() {
    this->max_vert = 10000;
    this->used_vert = 0;
  };
  unsigned max_vert;
  unsigned used_vert;
  static std::shared_ptr<Batch> inst;
  std::vector<std::shared_ptr<Model>> list;
};
#endif // !BATCH_HPP
