# Sampa City - Projeto em 3D

O **Sampa City** é um projeto em 3D que simula a construção de uma cidade com prédios representados por cubos. A cidade é gerada de forma procedural, podendo ser alterada com base em uma semente específica. Os prédios são coloridos, tendo a opção de gerar cores aleatoriamente ou definir uma única cor para todos os prédios. Além disso, há a opção de clicar em um botão de aleatorização, que troca automaticamente as sementes, alterando as posições e tamanhos dos prédios. É possível definir o número de prédios que aparecem em cada quadrante. Além disso, podemos controlar as janelas dos prédios, mudando largura, profundidade e offset da janela, além de alterar se a mesma está ligada ou desligada. Há também um balão, que se move ao redor da cidade, podendo ser esse movimento horário ou anti-horário.

## Autores
- Rodrigo Hisashi Takeuti (RA: 11201921914)
- Lucas Dantas de Lima da Silva (RA: 11201921341)

## Link WEB
O link web se encontra [aqui](https://rodrigohisashi.github.io/cidade-projeto-computacao-grafica/sampa/).

## Controles

É possível controlar a câmera através do teclado:

- Arrow Up = Subir câmera
- Arrow Down = Descer câmera
- Arrow Left / A = Girar câmera para esquerda
- Arrow Right / D = Girar cãmera para direita
- Q = Mover câmera para esquerda
- E = Mover câmera para direita


## Descrição da Implementação

Temos as classes \`Balloon\`, \`Camera\`, \`Ground\`, \`Predio\` e \`Window\`, com as seguintes funções:
- \`Balloon\`: Classe que gera e movimenta o balão;
- \`Camera\`: Classe que gera e gerencia a câmera e seus movimentos;
- \`Ground\`: Classe que desenha e gera o chão;
- \`Predio\`: Classe que gera um prédio;
- \`Window\`: Classe que gerencia a visão do usuário e interação entre o usuário e os gráficos apresentados.

### \`Camera\`
Essa classe é a mesma utilizada para o projeto lookat apresentado em classe. As únicas alterações feitas foram a introdução do método \`tilt(float speed)\`, que  serve para movimentar a câmera para cima e para baixo, e a posição inicial da câmera, alterada no arquivo .hpp. 
No caso de \`tilt(float speed)\`, a implementação foi feita alterando \`m_eye\` e \`m_at\` usando o vetor \`m_up\`:
```cpp
void Camera::tilt(float speed) {
  // Mover câmera para cima e para baixo
  m_eye += m_up * speed;
  m_at += m_up * speed;

  computeViewMatrix();
}
```
A posição inicial da câmera foi alterada da seguinte forma:
```cpp
glm::vec3 m_eye{0.0f, 8.0f, 8.5f}; // Camera position
glm::vec3 m_at{0.0f, 1.25f, 0.0f}; // Look-at point
glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction
```

### \`Balloon\` e \`Predio\`
Ambas classes são baseadas na classe \`Model\` apresentada no projeto Viewer, no caso de \`Predio\` não houve alterações significativas, já no caso de \`Balloon\` foram adicionados 3 métodos \`dolly(float speed)\`, \`truck(float speed)\` e \`tilt(float speed)\`, e 3 atributos \`m_position\`, \`m_direction\` e \`m_up\`

#### Alterações na Classe \`Balloon\`
Os métodos adicionados são relacionados ao movimento do carro, sendo \`dolly(float speed)\` utilizado para o movimento para frente e para trás, \`truck(float speed)\` utilizado para o movimento para direita e esquerda e \`tilt(float speed)\` para o movimento para cima e para baixo. Esses métodos foram adaptados da classe \`Camera\`
```cpp
void Balloon::dolly(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_position - m_direction)};

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_position += forward * speed;
  m_direction += forward * speed;
  
}

void Balloon::truck(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_position - m_direction)};
  // Compute vector to the left
  auto const left{glm::cross(m_up, forward)};

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_position -= left * speed;
  m_direction -= left * speed;
}

void Balloon::tilt(float speed) {

  // Move balloon up and down
  m_position += m_up * speed;
  m_direction += m_up * speed;
}
```
A posição do balão foi definida como {0.0f, 3.0f, 0.0f} para que ele parta do centro da cidade de um ponto elevado e a direção foi definida {0.0f, 3.0f, -1.0f}, para que ele inicie se dirigindo a direção negativa do eixo z.

### \`Ground\`
Essa classe foi baseada na classe de mesmo nome apresentada no projeto lookat. A classe segue o mesmo príncipio, mantendo o mesmo método \`create(GLuint program)\`, porém, alteramos o método \`paint()\`, de forma que pintamos o quadrado de cinza escuro caso esteja na borda ou na cruz central, de forma a desenhar uma rua, e cinza claro caso contrário, representando o espaço onde os prédios serão desenhados.
```cpp
void Ground::paint() {
  abcg::glBindVertexArray(m_VAO);

  // Draw a grid of 2N+1 x 2N+1 tiles on the xz plane, centered around the
  // origin
  auto const N{5};
  for (auto const z : iter::range(-N, N + 1)) {
    for (auto const x : iter::range(-N, N + 1)) {
      // Set model matrix as a translation matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.0f, z));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      // Set color (light gray for the rest, dark gray for border and center
      // cross)
      auto const isCenter = (z == 0 || x == 0);
      auto const isBorder = (z == N || z == -N || x == N || x == -N);
      auto const gray = isCenter || isBorder ? 0.2f : 0.8f;
      abcg::glUniform4f(m_colorLoc, gray, gray, gray, 1.0f);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
}
```

### \`Window\`
Essa classe, responsável por gerenciar a janela de renderização e interação com o usuário foi aquela em que houve o maior número de mudanças. Passaremos pelos métodos e suas alterações.
#### \`onEvent(SDL_Event const &event)\`
No caso desse método, somente houve alteração para que as setas do teclado gerenciem o movimento para cima e para baixo.

#### \`onCreate()\`
No caso desse método, alteramos para carregar os modelos 3D e inicializar os valores de algumas váriaveis usadas para geração dos prédios e janelas.

#### \`gerarAndaresPorPredio(int num_building, int seed)\`, \`gerarLarguraProfundidadeAleatorio(int num_building, int seed)\`, \`gerarCoresAleatorias(int numBuildings)\` e \`generateRandomBuildingPositions(int numBuildings, int seed, float a, float b, float c, float d)\`
Esses métodos foram criados por nós para gerar aleatoriamente, respectivamente, o número de andares para cada prédio, valores para largura e profundidade dos prédios, cores aleatórias para os prédios e posições para os prédios dentro de uma área específica com base em uma semente. Esses métodos seguem a mesma estrutura, mudando somente os intervalos de geração. Assim, segue o exemplo do método \`gerarAndaresPorPredio(int num_building, int seed)\`:
```cpp
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
```

#### \`isPositionValid(const std::vector<glm::vec3> &positions, const glm::vec3 &newPosition, float radius)\`
Esse método é usado pelo método \`generateRandomBuildingPositions(int numBuildings, int seed, float a, float b, float c, float d)\` para que não ocorra uma grande sobreposição entre diferentes prédios.
```cpp
bool Window::isPositionValid(const std::vector<glm::vec3> &positions,
                             const glm::vec3 &newPosition, float radius) {
  for (const auto &position : positions) {
    if (glm::distance(newPosition, position) < 2 * radius) {
      return false;
    }
  }
  return true;
}
```

#### \`fazerJanela(glm::vec3 buildingPosition, float buildingWidth, float buildingDepth, int floor, float windowWidth, float windowDepth, float windowOffsetX, float windowOffsetZ)\`
Esse método é utilizado para gerar as janelas nos prédios, ela usa o mesmo modelo do prédio, box.obj, e cria uma janela por andar, com o tamanho e características podendo serem alteradas no ImGui.
```cpp
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
```

#### \`onPaint()\`
Esse método é responsável por gerar os objetos na tela.
Primeiramente calculamos as caracteristicas dos prédios usando os métodos de geração automática:
```cpp
  building_positions = generateRandomBuildingPositions(
  num_building, m_seed, -4.25, -0.75, -4.25, -0.75);

  std::vector<std::vector<glm::vec3>> additional_building_positions = {
      generateRandomBuildingPositions(num_building, m_seed, 4.25, 0.75, 4.25,
                                      0.75),
      generateRandomBuildingPositions(num_building, m_seed, -4.25, -0.75, 4.25,
                                      0.75),
      generateRandomBuildingPositions(num_building, m_seed, 4.25, 0.75, -4.25,
                                      -0.75)};

  for (const auto &v : additional_building_positions) {
    building_positions.insert(building_positions.end(), v.begin(), v.end());
  }

  num_andares_por_predio = gerarAndaresPorPredio(num_building * 4, m_seed);
  num_largura = gerarLarguraProfundidadeAleatorio(num_building * 4, m_seed);
  num_profundidade =
      gerarLarguraProfundidadeAleatorio(num_building * 4, m_seed);
  cores_aleatorias = gerarCoresAleatorias(num_building * 4);building_positions = generateRandomBuildingPositions(
      num_building, m_seed, -4.25, -0.75, -4.25, -0.75);

  std::vector<std::vector<glm::vec3>> additional_building_positions = {
      generateRandomBuildingPositions(num_building, m_seed, 4.25, 0.75, 4.25,
                                      0.75),
      generateRandomBuildingPositions(num_building, m_seed, -4.25, -0.75, 4.25,
                                      0.75),
      generateRandomBuildingPositions(num_building, m_seed, 4.25, 0.75, -4.25,
                                      -0.75)};

  for (const auto &v : additional_building_positions) {
    building_positions.insert(building_positions.end(), v.begin(), v.end());
  }

  num_andares_por_predio = gerarAndaresPorPredio(num_building * 4, m_seed);
  num_largura = gerarLarguraProfundidadeAleatorio(num_building * 4, m_seed);
  num_profundidade =
      gerarLarguraProfundidadeAleatorio(num_building * 4, m_seed);
  cores_aleatorias = gerarCoresAleatorias(num_building * 4);
```
Em seguidam geramos os prédios um a um:
```cpp
for (int j = 0; j < num_building * 4; j++) {
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
```
Por fim geramos o balão e o chão:
```cpp
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, m_balloon.m_position);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  m_balloon.render();

  // Draw ground
  m_ground.paint();
```

#### \`onPaintUI()\`
Nesse método, geramos algumas janelas para interação com o usuário.
Temos primeiramente uma janela que permite que o usuário altere informações relacionadas aos prédios, como cores, semente de geração, etc.:
```cpp
{
    // Configura a posição e o tamanho da janela
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(windowX, windowY), ImGuiCond_FirstUseEver);
    ImGui::Begin("Predios");
    // Original sliders
    {
      ImGui::SliderInt("Seed", &m_seed, 0, 100, "Seed: %d");

      ImGui::SliderInt("Predio", &num_building, 0, 15, "Numero de predios: %d");
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
```
Outra janela é usada para alterar os atributos da janela:
```cpp
{
    ImVec2 windowSize(250, 150);
    float newWindowX = screenWidth - windowSize.x - 5;
    // Define a posição Y
    float newWindowY = 450;
    // Configura a posição e o tamanho da janela
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY), ImGuiCond_FirstUseEver);
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
```
Por fim, há uma janela que mostra a posição do balão e muda seu movimento para horário ou anti-horário:
```cpp
{
    ImVec2 windowSize(200, 150);
    float newWindowX = 5;
    // Define a posição Y
    float newWindowY = 140;
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY), ImGuiCond_FirstUseEver);

    // Iniciando uma nova janela ImGui
    ImGui::Begin("Balão");

    // Obtendo a posição do balloon
    auto balloonPosition = m_balloon.m_position; // ou m_balloon.m_position, dependendo de como você implementou

    // Mostrando a posição do balloon
    ImGui::Text("Posição do Balão:");
    ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", balloonPosition.x, balloonPosition.y, balloonPosition.z);

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
```

#### \`onUpdate()\`
Nesse método, é atualizado as posições da câmera e do balão.
```cpp
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
```

#### \`updateBalloonSpeed()\`
Essa função é usada para alterar o movimento do balão, fazendo o movimento horário e anti-horário, e o movimento para cima e para baixo:
```cpp
void Window::updateBalloonSpeed() {

  if (m_balloon.m_position.z > -5.0f
   && m_balloon.m_position.z < 5.0f
   && m_balloon.m_position.x > -0.5f
   && m_balloon.m_position.x < 0.5f) {
    m_balloon_dollySpeed = m_movimento * 0.5f;
    m_balloon_truckSpeed = 0.0f;
  }
  if (m_movimento == 1) {
    if (m_balloon.m_position.z >= 5.0f
     && m_balloon.m_position.x < 5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = -0.05f;
    }
    if (m_balloon.m_position.z > -5.0f
     && m_balloon.m_position.x >= 5.0f) {
      m_balloon_dollySpeed = -0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
    if (m_balloon.m_position.z <= -5.0f
     && m_balloon.m_position.x > -5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = 0.05f;
    }
    if (m_balloon.m_position.z < 5.0f
     && m_balloon.m_position.x <= -5.0f) {
      m_balloon_dollySpeed = 0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
  } else {
    if (m_balloon.m_position.z >= 5.0f
     && m_balloon.m_position.x > -5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = 0.05f;
    }
    if (m_balloon.m_position.z < 5.0f
     && m_balloon.m_position.x >= 5.0f) {
      m_balloon_dollySpeed = 0.5f;
      m_balloon_truckSpeed = 0.0f;
    }
    if (m_balloon.m_position.z <= -5.0f
     && m_balloon.m_position.x < 5.0f) {
      m_balloon_dollySpeed = 0.0f;
      m_balloon_truckSpeed = -0.05f;
    }
    if (m_balloon.m_position.z > -5.0f
     && m_balloon.m_position.x <= -5.0f) {
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
```



