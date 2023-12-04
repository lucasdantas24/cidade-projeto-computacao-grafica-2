#ifndef PREDIO_HPP_
#define PREDIO_HPP_

#include "abcgOpenGL.hpp"

#include "janela.hpp"
#include "model.hpp"
#include <chrono>

class Predio {
public:
  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint PredioProgram{};

  Janela m_janela;
  Model m_janela_model;

  GLuint diffuseTexture{};

  GLint PredioViewMatrixLocation{};
  GLint PredioProjMatrixLocation{};
  GLint PredioModelMatrixLocation{};
  GLint PredioColorLocation{};

  glm::vec4 lightColor{}; // Luz branca
  glm::vec4 lightPos{};   // Posição da luz
  glm::vec4 Ia{};                           // Intensidade da luz ambiente
  glm::vec4 Ka{}; // Coeficiente de reflexão ambiente
  glm::vec4 Id{};                   // Intensidade difusa
  glm::vec4 Is{};                   // Intensidade especular
  glm::vec4 Kd{};                   // Coeficiente de reflexão difusa
  glm::vec4 Ks{};                   // Coeficiente de reflexão especular
  float shininess{};               // Brilho especular

  std::vector<glm::vec3> building_positions;
  std::vector<int> num_andares_por_predio;
  std::vector<float> num_largura;
  std::vector<float> num_profundidade;

  std::vector<glm::vec4> cores_aleatorias;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void create(Model m_model, const std::string assetsPath);
  void paint(glm::mat4 viewMatrix, glm::mat4 projMatrix, Model m_model,
             int m_seed, int num_building, std::array<float, 4> m_clearColor,
             bool cores_random, float windowWidth, float windowDepth,
             float windowOffsetX, float windowOffsetZ, bool janelas_acesas);
  void update(glm::vec4 lightColorParam, glm::vec3 LightPosParam);
  void destroy();

  float calcularValorY(int i);

  bool isPositionValid(const std::vector<glm::vec3> &positions,
                       const glm::vec3 &newPosition, float radius);
  std::vector<glm::vec3> generateRandomBuildingPositions(int numBuildings,
                                                         int seed, float a,
                                                         float b, float c,
                                                         float d);
  std::vector<int> gerarAndaresPorPredio(int num_building, int seed);
  std::vector<float> gerarLarguraProfundidadeAleatorio(int num_building,
                                                       int seed);
  std::vector<glm::vec4> gerarCoresAleatorias(int numBuildings);
};

#endif