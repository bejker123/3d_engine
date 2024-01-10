#include <GLFW/glfw3.h>
#include <optional>
#include <string>

namespace En {
namespace GLFW {

#define DEBUG_INFO 0
bool setup();
std::optional<std::string> get_error();
void terminate();
void debug_info();

} // namespace GLFW
} // namespace En
