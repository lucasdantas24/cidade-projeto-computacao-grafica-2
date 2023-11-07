#include "window.hpp"

#include <random>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_UP)
      m_tiltSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN)
      m_tiltSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_w) && m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_s) && m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_UP) && m_tiltSpeed > 0)
      m_tiltSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN) && m_tiltSpeed < 0)
      m_tiltSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

std::vector<int> Window::gerarAndaresPorPredio(int num_building) {
  std::vector<int> num_andares_por_predio;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(
      1, 5); // Ajuste o intervalo conforme necessário

  for (int j = 0; j < num_building; j++) {
    int num_andar =
        dis(gen); // Gera um número aleatório de andares para cada prédio
    num_andares_por_predio.push_back(num_andar);
  }

  return num_andares_por_predio;
}

std::vector<glm::vec3>
Window::generateRandomBuildingPositions(int numBuildings) {
  std::vector<glm::vec3> positions;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> disX(-4.5f, -0.2f);
  std::uniform_real_distribution<float> disZ(-4.5f, -0.2f);

  for (int i = 0; i < numBuildings; ++i) {
    glm::vec3 newPos;
    do {
      float posX = disX(gen);
      float posZ = disZ(gen);
      newPos = glm::vec3(posX, 0.0f, posZ);
    } while (!isPositionValid(positions, newPos, 0.25f));

    positions.emplace_back(newPos);
  }

  return positions;
}

bool Window::isPositionValid(const std::vector<glm::vec3> &positions,
                             const glm::vec3 &newPosition, float radius) {
  for (const auto &position : positions) {
    if (glm::distance(newPosition, position) < 2 * radius) {
      return false;
    }
  }
  return true;
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);

  m_predio.loadObj(assetsPath + "box.obj");
  m_predio.setupVAO(m_program);
  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  num_building = 10;
  // create vector
  building_positions = generateRandomBuildingPositions(num_building);
  num_andares_por_predio = gerarAndaresPorPredio(num_building);
  // Load model
  loadModelFromFile(assetsPath + "P44.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}
float Window::calcularValorY(int i) { return 0.6f * (i + 1); }

void Window::fazerJanela(glm::vec3 posicao_predio, GLuint modelMatrixLoc) {

  // float x_predio = posicao_predio.x;
  // float y_predio = posicao_predio.y;
  // float z_predio = posicao_predio.z;

  // float x_janela = x_predio - 0.5f;
  // float y_janela = y_predio / 3.0f;
  // float z_janela = z_predio;

  // // Posições relativas das 4 janelas
  // glm::vec3 posicao_janela_1{x_janela, y_janela, z_janela};
  // glm::vec3 posicao_janela_2{x_janela, y_janela + 0.2f, z_janela};
  // glm::vec3 posicao_janela_3{x_janela + 0.2f, y_janela, z_janela};
  // glm::vec3 posicao_janela_4{x_janela + 0.2f, y_janela + 0.2f, z_janela};

  // glm::mat4 modelMatrix{1.0f};
  // modelMatrix = glm::translate(modelMatrix, posicao_janela_1);
  // modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
  // abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  // m_predio.render();
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};

  for (int j = 0; j < num_building; j++) {
    for (int i = 0; i < num_andares_por_predio.at(j); i++) {
      glm::mat4 modelMatrix{1.0f};
      glm::vec3 posicao_predio =
          glm::vec3(building_positions.at(j).x, calcularValorY(i),
                    building_positions.at(j).z);
      modelMatrix = glm::translate(modelMatrix, posicao_predio);
      modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

      fazerJanela(posicao_predio, modelMatrixLoc);

      m_predio.render();
    }
  }

  // for (int i = 0; i < num_building; ++i) {
  //   glm::mat4 model{1.0f};
  //   model = glm::translate(model, building_positions.at(i));
  //   model = glm::scale(model, glm::vec3(0.025f));

  //   abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE,
  //   &model[0][0]);
  //   abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  //   abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                        nullptr);
  // }
  // // Draw white bunny
  // glm::mat4 model{1.0f};
  // model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
  // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  // model = glm::scale(model, glm::vec3(0.1f));

  // abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // // Draw yellow bunny
  // model = glm::mat4(1.0);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
  // model = glm::scale(model, glm::vec3(0.1f));

  // abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(m_colorLocation, 1.0f, 0.8f, 0.0f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // // Draw blue bunny
  // model = glm::mat4(1.0);
  // model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
  // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  // model = glm::scale(model, glm::vec3(0.1f));

  // abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(m_colorLocation, 0.0f, 0.8f, 1.0f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // // Draw red bunny
  // model = glm::mat4(1.0);
  // model = glm::scale(model, glm::vec3(0.1f));

  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paint();

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.tilt(m_tiltSpeed * deltaTime);
}