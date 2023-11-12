#ifndef BALLON_HPP_
#define BALLON_HPP_

#include "abcgOpenGL.hpp"
#include "predio.hpp"

class Ballon {
public:

  // Funções de renderização
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void destroy() const;

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  // Funções de movimento
  void dolly(float speed);
  void truck(float speed);
  void tilt(float speed);

  glm::vec3 m_position{0.0f, 3.0f, 0.0f}; // Posição do Balão
  glm::vec3 m_direction{0.0f, 3.0f, -1.0f}; // Direção do balão
  glm::vec3 m_up{0.0f, 10.0f, 0.0f};  // Vetor para cima

private:
  GLuint m_VAO, m_VBO, m_EBO;
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  
  void createBuffers();
  void standardize();
};

#endif