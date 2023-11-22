#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

class Ground {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void drawGround(int offsetX, int offsetZ);
  void drawRoundabout(int x, int z);
  void drawStreet(int x, int z);

private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif