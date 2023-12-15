#pragma once

#include "model.hpp"
#include <vector>

namespace En {

class Batch;
using pBatch = std::shared_ptr<Batch>;
class Batch {
public:
  static pBatch instance() {
    static pBatch b(new Batch);
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
  static pBatch inst;
  std::vector<pModel> list;
};

} // namespace En
