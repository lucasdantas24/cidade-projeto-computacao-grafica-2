#include "window.hpp"

#include <chrono>
#include <random>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    return abcg::hashCombine(h1, h2);
  }
};

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 2.0f;
    if (event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -2.0f;
    if (event.key.keysym.sym == SDLK_UP)
      m_tiltSpeed = 2.0f;
    if (event.key.keysym.sym == SDLK_DOWN)
      m_tiltSpeed = -2.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -2.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 2.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -2.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 2.0f;
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

auto lastTime = std::chrono::steady_clock::now();
void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = abcg::createOpenGLProgram(
      {{.source = assetsPath + "main.vert", .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "main.frag",
        .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);

  m_predio.create(m_model, assetsPath);

  m_balloon.loadObj(assetsPath + "Air_Balloon.obj");
  m_balloon.setupVAO(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  num_building = 240;
  // create vector
  // VARIAVEIS DA JANELA

  windowWidth = 0.422f; // Adjust window width
  windowDepth = 0.465f; // Adjust window depth
  windowOffsetX = 0.013;
  windowOffsetZ = windowDepth / 2.0f;
  // COR RANDOM
  cores_random = false;
  janelas_acesas = true;

  m_predio.lightColor = glm::vec4{0.13f, 0.5f, 0.44f, 0.0f};
  m_predio.lightPosWorldSpace = glm::vec4{0.0f, -10.0f, 0.0f, 0.0f};
  m_predio.Ia = glm::vec4{3.0f};
  m_predio.Ka = glm::vec4{0.2f, 0.2f, 0.2f, 1.0f};
  m_predio.Id = glm::vec4{0.5};
  m_predio.Kd = glm::vec4{1.0f};
  m_predio.Is = glm::vec4{1.5f};
  m_predio.Ks = glm::vec4{1.0f};
  m_predio.shininess = 100.0f;

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

  createSkybox();
  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onPaint() {

  abcg::glUseProgram(m_program);

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_balloon.m_position);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE,
                           &modelMatrix[0][0]);
  m_balloon.render();

  // Draw ground
  m_ground.paint();

  renderSkybox();

  m_predio.paint(m_camera.getViewMatrix(), m_camera.getProjMatrix(), m_model,
                 m_seed, num_building, m_clearColor, cores_random, windowWidth,
                 windowDepth, windowOffsetX, windowOffsetZ);

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  // Obtém a largura da tela
  float screenWidth = static_cast<float>(m_viewportSize.x);

  // Define a largura e a altura da janela
  ImVec2 windowSize(400, 150);

  // Calcula a posição X para começar no canto superior direito
  float windowX = screenWidth - windowSize.x - 5;

  // Define a posição Y
  float windowY = 0;

  // Create window for sliders
  {

    // Configura a posição e o tamanho da janela
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(windowX, windowY), ImGuiCond_FirstUseEver);
    ImGui::Begin("Predios");
    // Original sliders
    {
      ImGui::SliderInt("Seed", &m_seed, 0, 100, "Seed: %d");

      ImGui::SliderInt("Predio", &num_building, 0, 240,
                       "Numero de predios: %d");
    }
    // Checkbox to toggle randomization
    if (ImGui::Checkbox("Aleatorizando", &isRandomizing)) {
      if (isRandomizing) {
        lastTime = std::chrono::steady_clock::now();
      }
    }

    // Randomize m_seed if checkbox is checked and 0.5 second have passed
    if (isRandomizing) {
      auto currentTime = std::chrono::steady_clock::now();
      auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                             currentTime - lastTime)
                             .count();
      if (elapsedTime >= 0.5) {
        m_seed = std::rand() % 101; // Generate a random seed between 0 and 50
        lastTime = currentTime;
      }
    }

    ImGui::SameLine();

    ImGui::End();
  }

  {
    ImVec2 windowSize(250, 150);
    float newWindowX = screenWidth - windowSize.x - 5;
    // Define a posição Y
    float newWindowY = 450;
    // Configura a posição e o tamanho da janela
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY),
                            ImGuiCond_FirstUseEver);
    ImGui::Begin("Janela");
    ImGui::Text("Variaveis das janelas:");
    {
      ImGui::SliderFloat("Largura", &windowWidth, 0.1f, 2.0f);
      ImGui::SliderFloat("Profundidade", &windowDepth, 0.1f, 2.0f);
      ImGui::SliderFloat("OffsetX", &windowOffsetX, -2.0f, 2.0f);
      ImGui::SliderFloat("OffsetZ", &windowOffsetZ, -2.0f, 2.0f);
    }

    ImGui::End();
  }

  {
    ImVec2 windowSize(160, 60);
    float newWindowX = 5;
    // Define a posição Y
    float newWindowY = 75;
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY),
                            ImGuiCond_FirstUseEver);
    ImGui::Begin("Camera");
    if (ImGui::Button("Resetar a camera")) {
      m_camera.reset();
    }
    ImGui::End();
  }

  {
    ImVec2 windowSize(200, 150);
    float newWindowX = 5;
    // Define a posição Y
    float newWindowY = 140;
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY),
                            ImGuiCond_FirstUseEver);

    // Iniciando uma nova janela ImGui
    ImGui::Begin("Balão");

    // Obtendo a posição do balloon
    auto balloonPosition =
        m_balloon.m_position; // ou m_balloon.m_position, dependendo de como
                              // você implementou

    // Mostrando a posição do balloon
    ImGui::Text("Posição do Balão:");
    ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", balloonPosition.x,
                balloonPosition.y, balloonPosition.z);

    ImGui::Text("Movimento Balão:");

    if (ImGui::RadioButton("Anti-horário", m_movimento == 1)) {
      m_movimento = 1;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Horário", m_movimento == -1)) {
      m_movimento = -1;
    }

    ImGui::End();
  }

  // Dentro do método onPaintUI()
  {
    // Defina o tamanho e a posição da nova janela
    ImVec2 windowSize(300, 250);
    float newWindowX = 5;
    float newWindowY = 210;

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY),
                            ImGuiCond_FirstUseEver);

    // Inicie uma nova janela ImGui para os parâmetros de iluminação do prédio
    ImGui::Begin("Parâmetros Iluminação Prédio");

    // Adicione sliders para ajustar os atributos de iluminação
    // Dentro do bloco de código para criar a nova janela no ImGui
    ImGui::SliderFloat("Cor da Luz (R)", &m_predio.lightColor.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Cor da Luz (G)", &m_predio.lightColor.y, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Cor da Luz (B)", &m_predio.lightColor.z, 0.0f, 1.0f, "%.2f");

    ImGui::SliderFloat("Posição da Luz x", &m_predio.lightPosWorldSpace.x, -10.0f, 10.0f,"%.2f");
    ImGui::SliderFloat("Posição da Luz y", &m_predio.lightPosWorldSpace.y, -20.0f, 0.0f,"%.2f");
    ImGui::SliderFloat("Posição da Luz z", &m_predio.lightPosWorldSpace.z, -10.0f, 10.0f,"%.2f");
    ImGui::SliderFloat("Intensidade Ambiente", &m_predio.Ia.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat4("Reflexão Ambiente", &m_predio.Ka.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Intensidade Difusa", &m_predio.Id.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Reflexão Difusa", &m_predio.Kd.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Intensidade Especular", &m_predio.Is.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Reflexão Especular", &m_predio.Ks.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Brilho Especular", &m_predio.shininess, 1.0f, 150.0f, "%.2f");

    // Finalize a janela ImGui
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();
  m_predio.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  updateBalloonSpeed();

  m_balloon.dolly(m_balloon_dollySpeed * deltaTime);
  m_balloon.truck(m_balloon_truckSpeed * deltaTime);
  m_balloon.tilt(m_balloon_tiltSpeed * deltaTime);

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.tilt(m_tiltSpeed * deltaTime);
}

void Window::updateBalloonSpeed() {

  if (m_balloon.m_position.z > -5.0f && m_balloon.m_position.z < 5.0f &&
      m_balloon.m_position.x > -0.5f && m_balloon.m_position.x < 0.5f) {
    m_balloon_dollySpeed = m_movimento * 0.5f;
    m_balloon_truckSpeed = 0.0f;
  }
  if (m_movimento == 1) {
    if (m_balloon.m_position.z >= 5.0f && m_balloon.m_position.x < 5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = -0.05f;
    }
    if (m_balloon.m_position.z > -5.0f && m_balloon.m_position.x >= 5.0f) {
      m_balloon_dollySpeed = -0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
    if (m_balloon.m_position.z <= -5.0f && m_balloon.m_position.x > -5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = 0.05f;
    }
    if (m_balloon.m_position.z < 5.0f && m_balloon.m_position.x <= -5.0f) {
      m_balloon_dollySpeed = 0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
  } else {
    if (m_balloon.m_position.z >= 5.0f && m_balloon.m_position.x > -5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = 0.05f;
    }
    if (m_balloon.m_position.z < 5.0f && m_balloon.m_position.x >= 5.0f) {
      m_balloon_dollySpeed = 0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
    if (m_balloon.m_position.z <= -5.0f && m_balloon.m_position.x < 5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = -0.05f;
    }
    if (m_balloon.m_position.z > -5.0f && m_balloon.m_position.x <= -5.0f) {
      m_balloon_dollySpeed = -0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
  }

  if (m_balloon.m_position.y > 3.5f) {
    m_balloon_tiltSpeed = m_balloon_tiltSpeed * -1;
  }
  if (m_balloon.m_position.y < 2.5f) {
    m_balloon_tiltSpeed = m_balloon_tiltSpeed * -1;
  }
}

void Window::createSkybox() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Create skybox program
  auto const path{assetsPath + m_skyShaderName};
  m_skyProgram = abcg::createOpenGLProgram(
      {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
       {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}});

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::renderSkybox() {
  abcg::glUseProgram(m_skyProgram);

  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  auto const skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  auto const assetsPath{abcg::Application::getAssetsPath() + "maps/cube/"};

  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, loadCubeTexture(assetsPath));

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}
void Window::destroySkybox() const {
  abcg::glDeleteProgram(m_skyProgram);
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
}

GLuint Window::loadCubeTexture(std::string const &path) {

  GLuint m_cubeTexture = abcg::loadOpenGLCubemap(
      {.paths = {path + "posx.jpg", path + "negx.jpg", path + "posy.jpg",
                 path + "negy.jpg", path + "posz.jpg", path + "negz.jpg"}});

  return m_cubeTexture;
}