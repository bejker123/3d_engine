#include "../../../io/window.hpp"
#include <memory>
#include <optional>
namespace En {
namespace Vulkan {

std::optional<std::shared_ptr<Window>> init();

void terminate();

}; // namespace Vulkan
} // namespace En
