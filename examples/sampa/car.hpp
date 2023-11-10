#ifndef CAR_HPP_
#define CAR_HPP_

#include "abcgOpenGL.hpp"
#include "predio.hpp"

class Car {
public:
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void destroy() const;

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void tilt(float speed);

  glm::vec3 m_position{0.0f, 0.1f, 4.0f}; // Camera position
  glm::vec3 m_direction{0.0f, 0.1f, 0.0f}; // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

private:
  glm::mat4 m_projMatrix;
  glm::mat4 m_viewMatrix;
  

    // Componentes de Predio para o modelo do carro
  GLuint m_VAO, m_VBO, m_EBO;
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  
  void createBuffers();
  void standardize();


};

#endif