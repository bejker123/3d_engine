#include "app.hpp"
#include "engine/shaders.hpp"
#include <memory>
#include <ranges>

namespace rv = std::ranges::views;

std::vector<En::ll::VertexC> vertices{
    // Front Face 0
    En::ll::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(1, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 1),
                    glm::vec4(1, 1, 0, 1)),

    // Top Fglm::vec3(ace 4
    En::ll::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 0),
                    glm::vec4(1, 1, 0, 1)),

    // Rightglm::vec3( Face 8
    En::ll::VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(0, 1),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                    glm::vec4(1, 1, 0, 1)),

    // Left glm::vec3(Face 12
    En::ll::VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(1.0f, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1, 0.0f),
                    glm::vec4(1, 1, 0, 1)),

    // Bottoglm::vec3(m Face 16
    En::ll::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1.0f, 1),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 1.0f),
                    glm::vec4(1, 1, 0, 1)),

    // Back glm::vec3(Face 20
    En::ll::VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1, 0),
                    glm::vec4(1, 1, 0, 1)),
    En::ll::VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0.0f, 0),
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

int App::pre_init(EN engine) { return 1; }

// MAKE GAME INHERETE FROM ENGINE AND OVERLOAD GAME ORIENTED FUNCTIONS
int App::init(EN engine) {
  // Reamember to set the camera's 'z-near' (near plane) as far as possible (1
  // is good enough)
  engine->cam.init(60, 1., 100000, glm::vec3(-40, 20, 30));
  engine->add_shader(camera_vs, basic_fs, "");
  engine->add_shader(camera_vs, cool_fs, "");

  va.init();

  vb.init(vertices);

  ib.init(indices, sizeof(indices));

  va.add_vertex_buffer(&vb);
  // va.addVertexBuffer(&vb1);
  va.set_index_buffer(&ib);

  En::Mesh mesh;
  mesh.init(std::make_shared<En::ll::VertexArray>(va));
  // mesh.load(
  //     "/home/bejker/Downloads/Survival_BackPack_2/Survival_BackPack_2.fbx");
  En::Material mat;
  mat.init(engine->get_shader(0).value());
  // TODO: Change this texture
  // Creating the texture "on the fly" makes is appear propely
  mat.set_texture(
      std::shared_ptr<En::ll::Texture>(new En::ll::Texture("face.png")));

  for (uint64_t i : rv::iota(0, 10)) {
    En::Model m;
    m.init(std::make_shared<En::Mesh>(mesh),
           std::make_unique<En::Material>(mat));
    m.set_origin(glm::vec3((double)i * 21, 0, 0));
    engine->add_model(m);
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
