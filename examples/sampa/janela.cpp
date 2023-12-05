#include "janela.hpp"
#include <glm/fwd.hpp>

void Janela::create(Model m_model, const std::string assetsPath) {
  // Cria um modelo com o shader do prédio

  JanelaProgram = abcg::createOpenGLProgram({{.source = assetsPath + "predio.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "predio.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Carregamos os índices e vértices para a bola a partir do sphere.obj
  m_model.loadObj(assetsPath + "box.obj", &m_vertices, &m_indices, &m_VBO,
                  &m_EBO);

  // Inicializamos os buffers para a parede
  m_model.setupVAO(JanelaProgram, &m_VBO, &m_EBO, &m_VAO);

  JanelaViewMatrixLocation =
      abcg::glGetUniformLocation(JanelaProgram, "viewMatrix");
  JanelaProjMatrixLocation =
      abcg::glGetUniformLocation(JanelaProgram, "projMatrix");

  JanelaColorLocation = abcg::glGetUniformLocation(JanelaProgram, "color");

  m_model.loadDiffuseTexture(assetsPath + "maps/Facade006_1K-JPG_Color.jpg",
                             &diffuseTexture);
}

void Janela::update(glm::vec4 lightColorParam, glm::vec3 LightPosParam) {
  // Acertamos a luz especular, "brilho", com a cor da luz incidente
  lightPosWorldSpace = glm::vec4(LightPosParam, 0);
  Is = lightColorParam;
  shininess = 2 * abs(LightPosParam.x);
}

void Janela::paint(glm::mat4 viewMatrix, glm::mat4 projMatrix, Model m_model,
                   glm::vec3 buildingPosition, float buildingWidth,
                   float buildingDepth, int floor, float windowWidth,
                   float windowDepth, float windowOffsetX, float windowOffsetZ) {

  abcg::glUseProgram(JanelaProgram);

  // Localização das matrizes
  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(JanelaProgram, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(JanelaProgram, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(JanelaProgram, "modelMatrix")};
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(JanelaProgram, "normalMatrix")};

  auto const lightLoc{abcg::glGetUniformLocation(JanelaProgram, "lightPosWorldSpace")};

  // Localização das propriedades de iluminação do sol
  auto const shininessLoc{
      abcg::glGetUniformLocation(JanelaProgram, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(JanelaProgram, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(JanelaProgram, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(JanelaProgram, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(JanelaProgram, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(JanelaProgram, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(JanelaProgram, "Ks")};

  // Bind das propriedades
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);

  // Propriedades da luz
  abcg::glUniform4fv(lightLoc, 1, &lightPosWorldSpace.x);
  abcg::glUniform4fv(IaLoc, 1, &Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &Id.x);
  abcg::glUniform4fv(IsLoc, 1, &Is.x);
  abcg::glUniform4fv(KaLoc, 1, &Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &Ks.x);
  abcg::glUniform1f(shininessLoc, shininess);

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

  auto modelViewMatrix{glm::mat3(viewMatrix * modelMatrix)};
  auto normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  // Update model matrix for window
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  m_model.renderTexture(&m_indices, &m_VAO, diffuseTexture);
  abcg::glUseProgram(0);
}

void Janela::destroy() {
  abcg::glDeleteProgram(JanelaProgram);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}