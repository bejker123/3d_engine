#include <memory>
#include <vulkan/vulkan.h>
namespace En {
class Vulkan {
public:
  static bool init();

  static std::shared_ptr<Vulkan> instance();

  static void terminate();

  Vulkan(){};

private:
  static std::shared_ptr<Vulkan> _instance;

  static VkInstance vk_instance;
};
} // namespace En
