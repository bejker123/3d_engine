#include "app.hpp"
#include "engine/io/model_loader.hpp"
#include "engine/rendering/material.hpp"
#include <ranges>

namespace rv = std::ranges::views;

std::vector<En::VertexC> vertices{
    // Front Face 0
    En::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(1, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 1),
                glm::vec4(1, 1, 0, 1)),

    // Top Fglm::vec3(ace 4
    En::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 0),
                glm::vec4(1, 1, 0, 1)),

    // Rightglm::vec3( Face 8
    En::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(0, 1),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                glm::vec4(1, 1, 0, 1)),

    // Left glm::vec3(Face 12
    En::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(1.0f, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1, 0.0f),
                glm::vec4(1, 1, 0, 1)),

    // Bottoglm::vec3(m Face 16
    En::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1.0f, 1),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 1.0f),
                glm::vec4(1, 1, 0, 1)),

    // Back glm::vec3(Face 20
    En::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1, 0),
                glm::vec4(1, 1, 0, 1)),
    En::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0.0f, 0),
                glm::vec4(1, 1, 0, 1)),
};

std::vector<uint32_t> indices = {
    0,  1,  3,  3,  1,  2,  // Front face
    7,  5,  6,  6,  5,  4,  // Top Face
    8,  9,  11, 11, 9,  10, // Right face
    12, 13, 15, 15, 13, 14, // Left face
    19, 17, 18, 18, 17, 16, // Bottom face
    20, 21, 22, 22, 21, 23  // Back face
};

int App::pre_init(EN engine) { return 1; }

// MAKE GAME INHERETE FROM ENGINE AND OVERLOAD GAME ORIENTED FUNCTIONS
int App::init(EN engine) {
  // Reamember to set the camera's 'z-near' (near plane) as far as possible (1
  // is good enough)
  engine->cam.init(60, 0.1, 10000, glm::vec3(-40, 20, 30));
  engine->load_shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  auto texture = pTexture(new En::Texture("textures/face.png"));
  En::Material mat(engine->get_shader(0).value(), texture);
  En::Mesh mesh(std::make_shared<En::Material>(mat), vertices, indices);
  // TODO: Change this texture
  // Creating the texture "on the fly" makes is appear propely
  mat.set_texture(texture);

  En::ModelLoader ml;

  auto m = ml.load(engine->get_shader(0).value(), "models/sb/sb.obj");
  m.set_origin(glm::vec3(-30, 0, 0));
  m.set_scale(glm::vec3(1));
  // m.set_rot(glm::vec3(-90, 0, 0));
  engine->add_model(m);

  auto m1 = ml.load(engine->get_shader(0).value(), "models/sk/sk.obj");
  m1.set_origin(glm::vec3(-85, 0, 0));
  m1.set_scale(glm::vec3(1));
  // m.set_rot(glm::vec3(-90, 0, 0));
  engine->add_model(m1);

  auto m_ = ml.load(engine->get_shader(0).value(),
                    "models/backpack/backpack.obj", true);
  m_.set_origin(glm::vec3(-50, 0, 0));
  engine->add_model(m_);

  for (uint64_t i : rv::iota(0, 10)) {
    engine->add_model(std::make_shared<En::Mesh>(mesh));
    auto lm = engine->get_last_model();
    if (lm.has_value())
      lm.value()->set_origin(glm::vec3((double)i * 21, 0, 0));
  }
  return 1;
}

int App::pre_update(EN engine) { return 1; }

int App::update(EN engine) { return 1; }

int App::post_update(EN engine) { return 1; }

void App::pre_render(EN engine) {}

void App::render(EN engine) {}

void App::post_render(EN engine) {}

void App::pre_terminate(EN engine) {}

void App::terminate(EN engine) {}
