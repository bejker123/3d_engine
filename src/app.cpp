#include "app.hpp"
#include "engine/engine.hpp"
#include "engine/prelude.hpp"
// #include "engine/shaders.hpp"
#include <iostream>
#include <memory>
#include <ranges>

namespace rv = std::ranges::views;

// MAKE GAME INHERETE FROM ENGINE AND OVERLOAD GAME ORIENTED FUNCTIONS
int App::init(Engine *engine) { return 1; }

int App::update(Engine *engine) { return 1; }

void App::render(Engine *engine) {}
