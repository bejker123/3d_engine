#include "../../io/window.hpp"
#include <memory>
#include <optional>
#include <string>
namespace En {
class Vulkan {
public:
  static std::optional<std::shared_ptr<Window>> init();

  static std::shared_ptr<Vulkan> instance();

  static void terminate();
  ~Vulkan();

private:
  Vulkan();
};
} // namespace En
