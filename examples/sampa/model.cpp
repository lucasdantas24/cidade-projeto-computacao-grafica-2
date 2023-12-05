#include "model.hpp"

#include <filesystem>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    return abcg::hashCombine(h1, h2);
  }
};

void Model::createBuffers(std::vector<Vertex> *m_vertices,
                          std::vector<GLuint> *m_indices, GLuint *m_VBO,
                          GLuint *m_EBO) {

  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  abcg::glDeleteBuffers(1, m_EBO);
  abcg::glDeleteBuffers(1, m_VBO);

  // VBO
  abcg::glGenBuffers(1, m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, *m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof((*m_vertices).at(0)) * (*m_vertices).size(),
                     (*m_vertices).data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof((*m_indices).at(0)) * (*m_indices).size(),
                     (*m_indices).data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::loadObj(std::string_view path, std::vector<Vertex> *m_vertices,
                    std::vector<GLuint> *m_indices, GLuint *m_VBO,
                    GLuint *m_EBO, bool standardize) {
  tinyobj::ObjReader reader;
  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  (*m_vertices).clear();
  (*m_indices).clear();

  m_hasNormals = false;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Vertex normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      Vertex const vertex{.position = position, .normal = normal};

      // If hash doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = (*m_vertices).size();
        // Add this vertex
        (*m_vertices).push_back(vertex);
      }

      (*m_indices).push_back(hash[vertex]);
    }
  }

  if (standardize) {
    Model::standardize(m_vertices);
  }

  if (!m_hasNormals) {
    computeNormals(m_vertices, m_indices);
  }

  createBuffers(m_vertices, m_indices, m_VBO, m_EBO);
}

void Model::render(std::vector<GLuint> *m_indices, GLuint *m_VAO,
                   int numTriangles) const {
  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  abcg::glBindVertexArray(*m_VAO);

  auto const numIndices{(numTriangles < 0) ? (*m_indices).size()
                                           : numTriangles * 3};

  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Model::renderTexture(std::vector<GLuint> *m_indices, GLuint *m_VAO,
                          GLuint diffuseTexture, int numTriangles) const {
  abcg::glBindVertexArray(*m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  auto const numIndices{(numTriangles < 0) ? (*m_indices).size()
                                           : numTriangles * 3};

  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Model::loadDiffuseTexture(std::string_view path, GLuint *diffuseTexture) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, diffuseTexture);
  *diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}

void Model::setupVAO(GLuint program, GLuint *m_VBO, GLuint *m_EBO,
                     GLuint *m_VAO) {
  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  // Release previous VAO
  abcg::glDeleteVertexArrays(1, m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, m_VAO);
  abcg::glBindVertexArray(*m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, *m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);
}

void Model::standardize(std::vector<Vertex> *m_vertices) {
  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  // Get bounds
  glm::vec3 max(std::numeric_limits<float>::lowest());
  glm::vec3 min(std::numeric_limits<float>::max());
  for (auto const &vertex : (*m_vertices)) {
    max = glm::max(max, vertex.position);
    min = glm::min(min, vertex.position);
  }

  // Center and scale
  auto const center{(min + max) / 2.0f};
  auto const scaling{2.0f / glm::length(max - min)};
  for (auto &vertex : (*m_vertices)) {
    vertex.position = (vertex.position - center) * scaling;
  }
}

void Model::computeNormals(std::vector<Vertex> *m_vertices,
                           std::vector<GLuint> *m_indices) {
  // Clear previous vertex normals
  for (auto &vertex : *m_vertices) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Compute face normals
  for (auto const offset :
       iter::range(0UL, (unsigned long)(*m_indices).size(), 3UL)) {
    // Get face vertices
    auto &a{(*m_vertices).at((*m_indices).at(offset + 0))};
    auto &b{(*m_vertices).at((*m_indices).at(offset + 1))};
    auto &c{(*m_vertices).at((*m_indices).at(offset + 2))};

    // Compute normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Accumulate on vertices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normalize
  for (auto &vertex : *m_vertices) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

void Model::destroy(GLuint *m_VBO, GLuint *m_EBO, GLuint *m_VAO) const {
  // Semelhante a implementação da aula, adicionando ponteiros para poder renderizar todos os modelos
  abcg::glDeleteBuffers(1, m_EBO);
  abcg::glDeleteBuffers(1, m_VBO);
  abcg::glDeleteVertexArrays(1, m_VAO);
}