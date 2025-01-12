#ifndef JANELA_HPP_
#define JANELA_HPP_

#include "abcgOpenGL.hpp"

#include "model.hpp"

class Janela {
public:
  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint JanelaProgram{};

  GLuint diffuseTexture{};

  GLint JanelaViewMatrixLocation{};
  GLint JanelaProjMatrixLocation{};
  GLint JanelaModelMatrixLocation{};
  GLint JanelaColorLocation{};

  glm::vec4 lightColor{0.0f, 1.0f, 0.0f, 0.0f};
  glm::vec4 lightPosWorldSpace{0.0f, 1.0f, 0.0f, 0.0f};
  glm::vec4 Ia{1.0f};
  glm::vec4 Ka{0.2f, 0.2f, 0.2f, 1.0f};
  glm::vec4 Id{1.0f};
  glm::vec4 Is{1.0f};
  glm::vec4 Kd{1.0f};
  glm::vec4 Ks{1.0f};
  float shininess{150.0f};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void create(Model m_model, const std::string assetsPath);
  void paint(glm::mat4 viewMatrix, glm::mat4 projMatrix, Model m_model, glm::vec3 buildingPosition, float buildingWidth,
                         float buildingDepth, int floor, float windowWidth,
                         float windowDepth, float windowOffsetX,
                         float windowOffsetZ);
  void update(glm::vec4 lightColorParam, glm::vec3 LightPosParam);
  void destroy();
};

#endif