#include "window.hpp"

#include <chrono>
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

std::vector<int> Window::gerarAndaresPorPredio(int num_building, int seed) {
  std::vector<int> num_andares_por_predio;

  {
    std::mt19937 gen(seed); // Use a semente fixa para inicializar o gerador de
                            // números pseudo-aleatórios
    std::uniform_int_distribution<> dis(
        1, 5); // Ajuste o intervalo conforme necessário

    for (int j = 0; j < num_building; j++) {
      int num_andar =
          dis(gen); // Gera um número aleatório de andares para cada prédio
      num_andares_por_predio.push_back(num_andar);
    }
  }

  return num_andares_por_predio;
}

std::vector<float> Window::gerarLarguraProfundidadeAleatorio(int num_building,
                                                             int seed) {
  std::vector<float> largura_profundidade;

  {
    std::mt19937 gen(seed); // Use a semente fixa para inicializar o gerador de
                            // números pseudo-aleatórios
    std::uniform_real_distribution<float> dis(
        0.4f, 0.8f); // Ajuste o intervalo conforme necessário

    for (int j = 0; j < num_building; j++) {
      float num =
          dis(gen); // Gera um número aleatório de andares para cada prédio
      largura_profundidade.push_back(num);
    }
  }

  return largura_profundidade;
}

std::vector<glm::vec4> Window::gerarCoresAleatorias(int numBuildings) {
  std::vector<glm::vec4> cores_aleatorias;
  srand(time(NULL)); // inicializa a semente do gerador de números aleatórios

  for (int i = 0; i < numBuildings; i++) {
    float r =
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) *
                                      2.0f); // Fator de escurecimento aplicado
    float g =
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) * 2.0f);
    float b =
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) * 2.0f);
    float a = 1.0f; // Alfa definido como 1.0 para opacidade total
    glm::vec4 cor(r, g, b, a);
    cores_aleatorias.push_back(cor);
  }

  return cores_aleatorias;
}

std::vector<glm::vec3>
Window::generateRandomBuildingPositions(int numBuildings, int seed, float a,
                                        float b, float c, float d) {
  std::vector<glm::vec3> positions;
  std::mt19937 gen(seed);
  std::uniform_real_distribution<float> disX(a, b);
  std::uniform_real_distribution<float> disZ(c, d);

  for (int i = 0; i < numBuildings; ++i) {
    glm::vec3 newPos;
    do {
      float posX = disX(gen);
      float posZ = disZ(gen);
      newPos = glm::vec3(posX, 0.0f, posZ);
    } while (!isPositionValid(positions, newPos, 0.3f));

    positions.emplace_back(newPos);
  }

  return positions;
}

bool Window::isPositionValid(const std::vector<glm::vec3> &positions,
                             const glm::vec3 &newPosition, float radius) {

  double integralX;
  double fractionalX = modf(newPosition.x, &integralX);
  int xPosition = (int) integralX;
  if (fractionalX > 0.5) xPosition++;
  double integralZ;
  double fractionalZ = modf(newPosition.z, &integralZ);
  int zPosition = (int) integralZ;
  if (fractionalZ > 0.5) zPosition++;
  auto const isCenter = (newPosition.z == 0 || newPosition.x == 0);
  auto const isBorder = (zPosition % 5 == 0 || xPosition % 5 == 0);
  bool valid = isCenter || isBorder ? false : true;
  for (const auto &position : positions) {
    if (glm::distance(newPosition, position) < 2 * radius) {
      valid = false;
    }
  }
  return valid;
}

auto lastTime = std::chrono::steady_clock::now();
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

  m_janela.loadObj(assetsPath + "box.obj");
  m_janela.setupVAO(m_program);

  m_balloon.loadObj(assetsPath + "Air_Balloon.obj");
  m_balloon.setupVAO(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  num_building = 540;
  // create vector
  // VARIAVEIS DA JANELA

  windowWidth = 0.422f; // Adjust window width
  windowDepth = 0.465f; // Adjust window depth
  windowOffsetX = 0.013;
  windowOffsetZ = windowDepth / 2.0f;
  // COR RANDOM
  cores_random = false;
  janelas_acesas = true;
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

float Window::calcularValorY(int i) { return 0.6f * (i + 1); }

void Window::fazerJanela(glm::vec3 buildingPosition, float buildingWidth,
                         float buildingDepth, int floor, float windowWidth,
                         float windowDepth, float windowOffsetX,
                         float windowOffsetZ) {

  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};

  // Calculate window position
  glm::vec3 windowPosition = buildingPosition;
  windowPosition.y += floor * 0.07f; // Adjust window height based on floor
  windowPosition.x += windowOffsetX;
  windowPosition.z += windowOffsetZ;

  // Check if window position is inside the building's volume
  if (windowPosition.x < buildingPosition.x - buildingWidth / 2.0f ||
      windowPosition.x > buildingPosition.x + buildingWidth / 2.0f ||
      windowPosition.z < buildingPosition.z - buildingDepth / 2.0f ||
      windowPosition.z > buildingPosition.z + buildingDepth / 2.0f) {
    // Skip rendering the window if it's outside the building
    return;
  }

  // Scale the window
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, windowPosition);
  modelMatrix =
      glm::scale(modelMatrix, glm::vec3(windowWidth, 0.5f, windowDepth));

  glm::vec4 windowColor;

  if (janelas_acesas) {
    windowColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // Example window color
  } else {
    windowColor = glm::vec4(0.0f, 0.0f, 0.0f,
                            1.0f); // Black color when janelas_acesas is false
  }

  abcg::glUniform4f(m_colorLocation, windowColor[0], windowColor[1],
                    windowColor[2], windowColor[3]);

  // Update model matrix for window
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  // Render the window cube
  m_janela.render();
}

void Window::onPaint() {

  building_positions = generateRandomBuildingPositions(
      num_building, m_seed, -15.0f, 15.0f, -15.0f, 15.0f);

  num_andares_por_predio = gerarAndaresPorPredio(num_building, m_seed);
  num_largura = gerarLarguraProfundidadeAleatorio(num_building, m_seed);
  num_profundidade =
      gerarLarguraProfundidadeAleatorio(num_building, m_seed);
  cores_aleatorias = gerarCoresAleatorias(num_building);
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
      modelMatrix = glm::scale(
          modelMatrix,
          glm::vec3(num_largura.at(j), 1.0f,
                    num_profundidade.at(j))); // Ajuste os valores de escala
      glm::vec4 cor;

      if (cores_random) {
        cor = cores_aleatorias.at(j);
      } else {
        // Cor fixa, por exemplo, cinza
        cor = glm::vec4(m_clearColor.at(0), m_clearColor.at(1),
                        m_clearColor.at(2), m_clearColor.at(3));
      }
      abcg::glUniform4f(m_colorLocation, cor[0], cor[1], cor[2], cor[3]);
      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

      m_predio.render();
      // Render windows on each floor

      fazerJanela(posicao_predio, num_largura.at(j), num_profundidade.at(j), i,
                  windowWidth, windowDepth, windowOffsetX, windowOffsetZ);
    }
  }

  abcg::glBindVertexArray(0);

  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_balloon.m_position);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  m_balloon.render();

  // Draw ground
  m_ground.paint();

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

      ImGui::SliderInt("Predio", &num_building, 0, 540, "Numero de predios: %d");
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

    ImGui::Text("Cores Aleatórias:");

    if (ImGui::RadioButton("Ligado", cores_random)) {
      cores_random = true;
      // Additional logic when "Ligado" is selected
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Desligado", !cores_random)) {
      cores_random = false;
      // Additional logic when "Desligado" is selected
    }

    // Editar a cor do predio

    ImGui::Text("Cor dos predios:");
    ImGui::ColorEdit3("Escolha a COR", m_clearColor.data());

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

    ImGui::Text("Janelas acesas:");

    if (ImGui::RadioButton("Ligado", janelas_acesas)) {
      janelas_acesas = true;
      // Additional logic when "Ligado" is selected
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Desligado", !janelas_acesas)) {
      janelas_acesas = false;
      // Additional logic when "Desligado" is selected
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
}

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
