#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "balloon.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "janela.hpp"
#include "model.hpp"
#include "predio.hpp"
#include <chrono>

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;
  void fazerJanela(glm::vec3 buildingPosition, float buildingWidth,
                   float buildingDepth, int floor, float windowWidth,
                   float windowDepth, float windowOffsetX, float windowOffsetZ);
  void updateBalloonSpeed();

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
  Balloon m_balloon;
  float m_balloon_dollySpeed{};
  float m_balloon_truckSpeed{};
  float m_balloon_tiltSpeed{-0.01f};
  int m_movimento{1};
  Ground m_ground;
  Predio m_predio;
  Janela m_janela;
  Model m_model;
  
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
  bool cores_random;
  bool janelas_acesas;
  std::array<float, 4> m_clearColor{0.906f, 0.920f, 0.918f, 1.0f};

  bool isRandomizing{false};

  int m_trianglesToDraw{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  GLuint predio_program{};

  // Skybox
  std::string const m_skyShaderName{"skybox"};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint m_skyProgram{};

  std::array<glm::vec3, 36> const m_skyPositions{{// Front
                                                  {-20, -20, +20},
                                                  {+20, -20, +20},
                                                  {+20, +20, +20},
                                                  {-20, -20, +20},
                                                  {+20, +20, +20},
                                                  {-20, +20, +20},
                                                  // Back
                                                  {+20, -20, -20},
                                                  {-20, -20, -20},
                                                  {-20, +20, -20},
                                                  {+20, -20, -20},
                                                  {-20, +20, -20},
                                                  {+20, +20, -20},
                                                  // Right
                                                  {+20, -20, -20},
                                                  {+20, +20, -20},
                                                  {+20, +20, +20},
                                                  {+20, -20, -20},
                                                  {+20, +20, +20},
                                                  {+20, -20, +20},
                                                  // Left
                                                  {-20, -20, +20},
                                                  {-20, +20, +20},
                                                  {-20, +20, -20},
                                                  {-20, -20, +20},
                                                  {-20, +20, -20},
                                                  {-20, -20, -20},
                                                  // Top
                                                  {-20, +20, +20},
                                                  {+20, +20, +20},
                                                  {+20, +20, -20},
                                                  {-20, +20, +20},
                                                  {+20, +20, -20},
                                                  {-20, +20, -20},
                                                  // Bottom
                                                  {-20, -20, -20},
                                                  {+20, -20, -20},
                                                  {+20, -20, +20},
                                                  {-20, -20, -20},
                                                  {+20, -20, +20},
                                                  {-20, -20, +20}}};

  // clang-format on

  void createSkybox();
  void renderSkybox();
  void destroySkybox() const;
  GLuint loadCubeTexture(std::string const &path);
};

#endif