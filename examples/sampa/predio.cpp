#include "predio.hpp"
#include <glm/fwd.hpp>

#include <chrono>
#include <random>
#include <unordered_map>

void Predio::create(Model m_model, const std::string assetsPath) {

  m_janela.create(m_janela_model, assetsPath);

  PredioProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "predio.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "predio.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Carregamos os índices e vértices para a bola a partir do sphere.obj
  m_model.loadObj(assetsPath + "box.obj", &m_vertices, &m_indices, &m_VBO,
                  &m_EBO);

  // Inicializamos os buffers para a parede
  m_model.setupVAO(PredioProgram, &m_VBO, &m_EBO, &m_VAO);

  PredioViewMatrixLocation =
      abcg::glGetUniformLocation(PredioProgram, "viewMatrix");
  PredioProjMatrixLocation =
      abcg::glGetUniformLocation(PredioProgram, "projMatrix");

  PredioColorLocation = abcg::glGetUniformLocation(PredioProgram, "color");

  m_model.loadDiffuseTexture(assetsPath + "maps/brick_base.jpg",
                             &diffuseTexture);
}

void Predio::update(glm::vec4 lightColorParam, glm::vec3 LightPosParam) {
  // Acertamos a luz especular, "brilho", com a cor da luz incidente
  lightPos = glm::vec4(LightPosParam, 0);
  Is = lightColorParam;
  shininess = 2 * abs(LightPosParam.x);
}

void Predio::paint(glm::mat4 viewMatrix, glm::mat4 projMatrix, Model m_model,
                   int m_seed, int num_building,
                   std::array<float, 4> m_clearColor, bool cores_random,
                   float windowWidth, float windowDepth, float windowOffsetX,
                   float windowOffsetZ, bool janelas_acesas) {

  abcg::glUseProgram(PredioProgram);

  building_positions = generateRandomBuildingPositions(
      num_building, m_seed, -15.0f, 15.0f, -15.0f, 15.0f);

  num_andares_por_predio = gerarAndaresPorPredio(num_building, m_seed);
  num_largura = gerarLarguraProfundidadeAleatorio(num_building, m_seed);
  num_profundidade = gerarLarguraProfundidadeAleatorio(num_building, m_seed);
  cores_aleatorias = gerarCoresAleatorias(num_building);

  // Localização das matrizes
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(PredioProgram, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(PredioProgram, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(PredioProgram, "modelMatrix")};
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(PredioProgram, "normalMatrix")};

  auto const lightLoc{abcg::glGetUniformLocation(PredioProgram, "lightPos")};

  // Localização das propriedades de iluminação do sol
  auto const shininessLoc{
      abcg::glGetUniformLocation(PredioProgram, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(PredioProgram, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(PredioProgram, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(PredioProgram, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(PredioProgram, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(PredioProgram, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(PredioProgram, "Ks")};

  // Bind das propriedades
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);

  // Propriedades da luz
  abcg::glUniform4fv(lightLoc, 1, &lightPos.x);
  abcg::glUniform4fv(IaLoc, 1, &Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &Id.x);
  abcg::glUniform4fv(IsLoc, 1, &Is.x);
  abcg::glUniform4fv(KaLoc, 1, &Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &Ks.x);
  abcg::glUniform1f(shininessLoc, shininess);

  for (int j = 0; j < num_building; j++) {
    for (int i = 0; i < num_andares_por_predio.at(j); i++) {
      abcg::glUseProgram(PredioProgram);
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
      abcg::glUniform4f(PredioColorLocation, cor[0], cor[1], cor[2], cor[3]);
      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

      m_model.renderTexture(&m_indices, &m_VAO, diffuseTexture);
      // Render windows on each floor

      m_janela.paint(viewMatrix, projMatrix, m_janela_model, posicao_predio,
                     num_largura.at(j), num_profundidade.at(j), i, windowWidth,
                     windowDepth, windowOffsetX, windowOffsetZ, janelas_acesas);
    }
  }
}

void Predio::destroy() {
  abcg::glDeleteProgram(PredioProgram);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

std::vector<int> Predio::gerarAndaresPorPredio(int num_building, int seed) {
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

std::vector<float> Predio::gerarLarguraProfundidadeAleatorio(int num_building,
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

std::vector<glm::vec4> Predio::gerarCoresAleatorias(int numBuildings) {
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
Predio::generateRandomBuildingPositions(int numBuildings, int seed, float a,
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

bool Predio::isPositionValid(const std::vector<glm::vec3> &positions,
                             const glm::vec3 &newPosition, float radius) {

  double integralX;
  double fractionalX = modf(newPosition.x, &integralX);
  int xPosition = (int)integralX;
  if (fractionalX > 0.5)
    xPosition++;
  double integralZ;
  double fractionalZ = modf(newPosition.z, &integralZ);
  int zPosition = (int)integralZ;
  if (fractionalZ > 0.5)
    zPosition++;
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

float Predio::calcularValorY(int i) { return 0.6f * (i + 1); }
