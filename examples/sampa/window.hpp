#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "ground.hpp"
#include "predio.hpp"

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
  void fazerJanela(glm::vec3 posicao_predio, GLuint modelMatrixLoc);

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
  Ground m_ground;
  Predio m_predio;
  std::vector<glm::vec3> building_positions;
  std::vector<int> num_andares_por_predio;
  struct PredioJanela {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  std::array<PredioJanela, 4> m_predioJanela;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool isPositionValid(const std::vector<glm::vec3> &positions,
                       const glm::vec3 &newPosition, float radius);
  std::vector<glm::vec3> generateRandomBuildingPositions(int numBuildings);
  std::vector<int> gerarAndaresPorPredio(int num_building);
  void loadModelFromFile(std::string_view path);
};

#endif