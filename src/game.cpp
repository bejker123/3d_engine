#include "game.hpp"
#include "engine/shaders.hpp"
#include <memory>
#include <ranges>

namespace rv = std::ranges::views;

std::vector<VertexC> vertices{
    // Front Face 0
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(1, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 1),
            glm::vec4(1, 1, 0, 1)),

    // Top Fglm::vec3(ace 4
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 0),
            glm::vec4(1, 1, 0, 1)),

    // Rightglm::vec3( Face 8
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(0, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
            glm::vec4(1, 1, 0, 1)),

    // Left glm::vec3(Face 12
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(1.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1, 0.0f),
            glm::vec4(1, 1, 0, 1)),

    // Bottoglm::vec3(m Face 16
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1.0f, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 1.0f),
            glm::vec4(1, 1, 0, 1)),

    // Back glm::vec3(Face 20
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0.0f, 0),
            glm::vec4(1, 1, 0, 1)),
};

unsigned int indices[] = {
    0,  1,  3,  3,  1,  2,  // Front face
    7,  5,  6,  6,  5,  4,  // Top Face
    8,  9,  11, 11, 9,  10, // Right face
    12, 13, 15, 15, 13, 14, // Left face
    19, 17, 18, 18, 17, 16, // Bottom face
    20, 21, 22, 22, 21, 23  // Back face
};
// MAKE GAME INHERETE FROM ENGINE AND OVERLOAD GAME ORIENTED FUNCTIONS
Game::Game() {
  this->engine = unique_ptr<Engine>(new Engine());
  this->engine->init();

  this->engine->cam.init(60, 0.0001, 100000, glm::vec3(-40, 20, 30));
  this->engine->add_shader(camera_vs, basic_fs, "");
  this->engine->add_shader(camera_vs, cool_fs, "");

  va.init();

  vb.init(vertices);

  ib.init(indices, sizeof(indices));

  va.add_vertex_buffer(&vb);
  // va.addVertexBuffer(&vb1);
  va.set_index_buffer(&ib);

  Mesh mesh;
  mesh.init(std::make_shared<VertexArray>(va));
  // mesh.load(
  //     "/home/bejker/Downloads/Survival_BackPack_2/Survival_BackPack_2.fbx");
  Material mat;
  mat.init(this->engine->get_shader(0).value());
  // TODO: Change this texture
  Texture tex("face.png");
  mat.set_texture(std::make_shared<Texture>(tex));

  for (uint64_t i : rv::iota(0, 10)) {
    Model m;
    m.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
    m.set_origin(glm::vec3((double)i * 21, 0, 0));
    this->engine->add_model(m);
  }

  while (this->update()) {
    this->render();
  }
}

int Game::update() {
  return this->engine->update() && !this->engine->window.should_close();
}

void Game::render() { this->engine->render(); }
