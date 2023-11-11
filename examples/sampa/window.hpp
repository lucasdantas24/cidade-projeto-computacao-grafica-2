#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "car.hpp"
#include "ground.hpp"
#include "predio.hpp"
#include <chrono>
// struct Vertex {
//   glm::vec3 position;

//   friend bool operator==(Vertex const &, Vertex const &) = default;
// };

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;
  float calcularValorY(int i);
  void fazerJanela(glm::vec3 buildingPosition, float buildingWidth,
                   float buildingDepth, int floor, float windowWidth,
                   float windowDepth, float windowOffsetX, float windowOffsetZ);

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};
  float m_tiltSpeed{};
  int num_building;
  Car m_car;
  Ground m_ground;
  Predio m_predio;
  Predio m_janela;
  std::vector<glm::vec3> building_positions;
  std::vector<int> num_andares_por_predio;
  std::vector<float> num_largura;
  std::vector<float> num_profundidade;

  std::vector<glm::vec4> cores_aleatorias;
  struct PredioJanela {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  std::array<PredioJanela, 4> m_predioJanela;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  int m_seed = 5;
  float windowWidth; // Adjust window width
  float windowDepth; // Adjust window depth
  float windowOffsetX;
  float windowOffsetZ;
  bool resizeableWindowOpen = false;
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

  bool isRandomizing;
  void loadModelFromFile(std::string_view path);

  void updateCar(float deltaTime);
  bool isAtCrossRoad(glm::vec3 position);
};

#endif