#include "keyboard.hpp"
#include <map>
#include <utility>

namespace En {

std::map<Key::Code, KeyState> keys;

void key_callback(GLFWwindow *win, int ikey, int scancode, int action,
                  int mods) {
  KeyState state{action};
  auto s = &keys.at(Key::Code(ikey));
  *s = state;
  // std::cout << KeyEvent{Key(ikey), scancode, keys.at(Key::Code(ikey)), mods};
}

void text_callback(GLFWwindow *win, unsigned int codepoint) {
  // LOG("char: %c\n", (char)codepoint);
}

Keyboard::Keyboard(GLFWwindow *win) {
  if (hooked)
    return;
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, text_callback);
  for (int i = -1; i < 349; i++) {
    if (i == 337)
      i = 340;
    else if (i == 315)
      i = 320;
    else if (i == 285)
      i = 290;
    else if (i == 270)
      i = 280;
    else if (i == 163)
      i = 256;
    else if (i == 97)
      i = 161;
    else if (i == 94)
      i = 96;
    else if (i == 62)
      i = 65;
    else if (i == 60)
      i = 61;
    else if (i == 58)
      i = 59;
    else if (i == 40)
      i = 44;
    else if (i == 33)
      i = 39;
    else if (i == 0)
      i = 32;
    keys.insert(std::pair(Key::Code(i), KeyState::RELEASE));
  }
  this->hooked = true;
}

const KeyState Keyboard(Key::Code code) { return keys.at(code); }

const bool Keyboard::get_key_pressed(const Key::Code code) const {
  return keys.at(code) == KeyState::PRESS;
}

const bool Keyboard::get_key_released(const Key::Code code) const {
  return keys.at(code) == KeyState::RELEASE;
}

const bool Keyboard::get_key_held(const Key::Code code) const {
  return keys.at(code) == KeyState::HOLD;
}

} // namespace En
