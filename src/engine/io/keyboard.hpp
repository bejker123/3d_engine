#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include <ostream>
namespace En {

enum KeyState : int {
  RELEASE = 0,
  PRESS = 1,
  HOLD = 2,
};

class Key {
public:
  enum Code : int16_t {
    KEY_UNKNOWN = -1,

    /* = Printable keys */
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHT_BRACKET = 93,
    KEY_GRAVE_ACCENT = 96,
    KEY_WORLD_1 = 161,
    KEY_WORLD_2 = 162,

    /* = Function keys */
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_F13 = 302,
    KEY_F14 = 303,
    KEY_F15 = 304,
    KEY_F16 = 305,
    KEY_F17 = 306,
    KEY_F18 = 307,
    KEY_F19 = 308,
    KEY_F20 = 309,
    KEY_F21 = 310,
    KEY_F22 = 311,
    KEY_F23 = 312,
    KEY_F24 = 313,
    KEY_F25 = 314,
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_MENU = 348,

  };
  Key(Code code) : code(code) {}
  Key(int16_t code) : code{code} {}

  constexpr const char *print() const {
    switch (this->code) {
    case Code::KEY_UNKNOWN:
      return "KEY_UNKNOWN";
      /* = Printable keys */
    case Code::KEY_SPACE:
      return "KEY_SPACE";
    case Code::KEY_APOSTROPHE:
      return "KEY_APOSTROPHE";
    case Code::KEY_COMMA:
      return "KEY_COMMA";
    case Code::KEY_MINUS:
      return "KEY_MINUS";
    case Code::KEY_PERIOD:
      return "KEY_PERIOD";
    case Code::KEY_SLASH:
      return "KEY_SLASH";
    case Code::KEY_0:
      return "KEY_0";
    case Code::KEY_1:
      return "KEY_1";
    case Code::KEY_2:
      return "KEY_2";
    case Code::KEY_3:
      return "KEY_3";
    case Code::KEY_4:
      return "KEY_4";
    case Code::KEY_5:
      return "KEY_5";
    case Code::KEY_6:
      return "KEY_6";
    case Code::KEY_7:
      return "KEY_7";
    case Code::KEY_8:
      return "KEY_8";
    case Code::KEY_9:
      return "KEY_9";
    case Code::KEY_SEMICOLON:
      return "KEY_SEMICOLON";
    case Code::KEY_EQUAL:
      return "KEY_EQUAL";
    case Code::KEY_A:
      return "KEY_A";
    case Code::KEY_B:
      return "KEY_B";
    case Code::KEY_C:
      return "KEY_C";
    case Code::KEY_D:
      return "KEY_D";
    case Code::KEY_E:
      return "KEY_E";
    case Code::KEY_F:
      return "KEY_F";
    case Code::KEY_G:
      return "KEY_G";
    case Code::KEY_H:
      return "KEY_H";
    case Code::KEY_I:
      return "KEY_I";
    case Code::KEY_J:
      return "KEY_J";
    case Code::KEY_K:
      return "KEY_K";
    case Code::KEY_L:
      return "KEY_L";
    case Code::KEY_M:
      return "KEY_M";
    case Code::KEY_N:
      return "KEY_N";
    case Code::KEY_O:
      return "KEY_O";
    case Code::KEY_P:
      return "KEY_P";
    case Code::KEY_Q:
      return "KEY_Q";
    case Code::KEY_R:
      return "KEY_R";
    case Code::KEY_S:
      return "KEY_S";
    case Code::KEY_T:
      return "KEY_T";
    case Code::KEY_U:
      return "KEY_U";
    case Code::KEY_V:
      return "KEY_V";
    case Code::KEY_W:
      return "KEY_W";
    case Code::KEY_X:
      return "KEY_X";
    case Code::KEY_Y:
      return "KEY_Y";
    case Code::KEY_Z:
      return "KEY_Z";
    case Code::KEY_LEFT_BRACKET:
      return "KEY_LEFT_BRACKET";
    case Code::KEY_BACKSLASH:
      return "KEY_BACKSLASH";
    case Code::KEY_RIGHT_BRACKET:
      return "KEY_RIGHT_BRACKET";
    case Code::KEY_GRAVE_ACCENT:
      return "KEY_GRAVE_ACCENT";
    case Code::KEY_WORLD_1:
      return "KEY_WORLD_1";
    case Code::KEY_WORLD_2:
      return "KEY_WORLD_2";

      /* = Function keys */
    case Code::KEY_ESCAPE:
      return "KEY_ESCAPE";
    case Code::KEY_ENTER:
      return "KEY_ENTER";
    case Code::KEY_TAB:
      return "KEY_TAB";
    case Code::KEY_BACKSPACE:
      return "KEY_BACKSPACE";
    case Code::KEY_INSERT:
      return "KEY_INSERT";
    case Code::KEY_DELETE:
      return "KEY_DELETE";
    case Code::KEY_RIGHT:
      return "KEY_RIGHT";
    case Code::KEY_LEFT:
      return "KEY_LEFT";
    case Code::KEY_DOWN:
      return "KEY_DOWN";
    case Code::KEY_UP:
      return "KEY_UP";
    case Code::KEY_PAGE_UP:
      return "KEY_PAGE_UP";
    case Code::KEY_PAGE_DOWN:
      return "KEY_PAGE_DOWN";
    case Code::KEY_HOME:
      return "KEY_HOME";
    case Code::KEY_END:
      return "KEY_END";
    case Code::KEY_CAPS_LOCK:
      return "KEY_CAPS_LOCK";
    case Code::KEY_SCROLL_LOCK:
      return "KEY_SCROLL_LOCK";
    case Code::KEY_NUM_LOCK:
      return "KEY_NUM_LOCK";
    case Code::KEY_PRINT_SCREEN:
      return "KEY_PRINT_SCREEN";
    case Code::KEY_PAUSE:
      return "KEY_PAUSE";
    case Code::KEY_F1:
      return "KEY_F1";
    case Code::KEY_F2:
      return "KEY_F2";
    case Code::KEY_F3:
      return "KEY_F3";
    case Code::KEY_F4:
      return "KEY_F4";
    case Code::KEY_F5:
      return "KEY_F5";
    case Code::KEY_F6:
      return "KEY_F6";
    case Code::KEY_F7:
      return "KEY_F7";
    case Code::KEY_F8:
      return "KEY_F8";
    case Code::KEY_F9:
      return "KEY_F9";
    case Code::KEY_F10:
      return "KEY_F10";
    case Code::KEY_F11:
      return "KEY_F11";
    case Code::KEY_F12:
      return "KEY_F12";
    case Code::KEY_F13:
      return "KEY_F13";
    case Code::KEY_F14:
      return "KEY_F14";
    case Code::KEY_F15:
      return "KEY_F15";
    case Code::KEY_F16:
      return "KEY_F16";
    case Code::KEY_F17:
      return "KEY_F17";
    case Code::KEY_F18:
      return "KEY_F18";
    case Code::KEY_F19:
      return "KEY_F19";
    case Code::KEY_F20:
      return "KEY_F20";
    case Code::KEY_F21:
      return "KEY_F21";
    case Code::KEY_F22:
      return "KEY_F22";
    case Code::KEY_F23:
      return "KEY_F23";
    case Code::KEY_F24:
      return "KEY_F24";
    case Code::KEY_F25:
      return "KEY_F25";
    case Code::KEY_KP_0:
      return "KEY_KP_0";
    case Code::KEY_KP_1:
      return "KEY_KP_1";
    case Code::KEY_KP_2:
      return "KEY_KP_2";
    case Code::KEY_KP_3:
      return "KEY_KP_3";
    case Code::KEY_KP_4:
      return "KEY_KP_4";
    case Code::KEY_KP_5:
      return "KEY_KP_5";
    case Code::KEY_KP_6:
      return "KEY_KP_6";
    case Code::KEY_KP_7:
      return "KEY_KP_7";
    case Code::KEY_KP_8:
      return "KEY_KP_8";
    case Code::KEY_KP_9:
      return "KEY_KP_9";
    case Code::KEY_KP_DECIMAL:
      return "KEY_KP_DECIMAL";
    case Code::KEY_KP_DIVIDE:
      return "KEY_KP_DIVIDE";
    case Code::KEY_KP_MULTIPLY:
      return "KEY_KP_MULTIPLY";
    case Code::KEY_KP_SUBTRACT:
      return "KEY_KP_SUBTRACT";
    case Code::KEY_KP_ADD:
      return "KEY_KP_ADD";
    case Code::KEY_KP_ENTER:
      return "KEY_KP_ENTER";
    case Code::KEY_KP_EQUAL:
      return "KEY_KP_EQUAL";
    case Code::KEY_LEFT_SHIFT:
      return "KEY_LEFT_SHIFT";
    case Code::KEY_LEFT_CONTROL:
      return "KEY_LEFT_CONTROL";
    case Code::KEY_LEFT_ALT:
      return "KEY_LEFT_ALT";
    case Code::KEY_LEFT_SUPER:
      return "KEY_LEFT_SUPER";
    case Code::KEY_RIGHT_SHIFT:
      return "KEY_RIGHT_SHIFT";
    case Code::KEY_RIGHT_CONTROL:
      return "KEY_RIGHT_CONTROL";
    case Code::KEY_RIGHT_ALT:
      return "KEY_RIGHT_ALT";
    case Code::KEY_RIGHT_SUPER:
      return "KEY_RIGHT_SUPER";
    case Code::KEY_MENU:
      return "KEY_MENU";
    default:
      return "KEY_UNKNOWN";
    }
  }

  constexpr const Code get_code() const { return this->code; }
  constexpr const int16_t get_code_int() const { return this->code; }

private:
  Code code;
};

struct KeyEvent {
  Key key;
  int scancode;
  int action;
  int mods;
  friend std::ostream &operator<<(std::ostream &os, const En::KeyEvent &m) {
    return os << m.key.print() << " " << m.mods << " " << m.action << " "
              << m.scancode << std::endl;
  }
};

class Keyboard {
public:
  Keyboard(){};
  Keyboard(GLFWwindow *win);

  const KeyState get_key_state(Key::Code code) const;
  const bool get_key_pressed(const Key::Code code) const;
  const bool get_key_released(const Key::Code code) const;
  const bool get_key_held(const Key::Code code) const;

private:
  bool hooked = false;
};
} // namespace En
