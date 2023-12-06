# Sampa City 2 - A cidade agora é outra! 

O **Sampa City 2** é um projeto em 3D que simula a construção de uma cidade com prédios representados por cubos, ela é a evoluçao do Sampa City 1.0. A cidade é gerada de forma procedural, podendo ser alterada com base em uma semente específica. Os prédios são texturizados com a textura brick_base da aula. Além disso, há a opção de clicar em um botão de aleatorização, que troca automaticamente as sementes, alterando as posições e tamanhos dos prédios. É possível definir o número de prédios que são renderizados. Além disso, podemos controlar as janelas dos prédios, mudando largura, profundidade e offset da janela. Há também um balão, que se move ao redor da cidade, podendo ser esse movimento horário ou anti-horário. Por fim, temos opção de alterar números da iluminação.

## Autores
- Rodrigo Hisashi Takeuti (RA: 11201921914)
- Lucas Dantas de Lima da Silva (RA: 11201921341)

## Link WEB
O link web se encontra [aqui](https://lucasdantas24.github.io/cidade-projeto-computacao-grafica-2/sampa/).

## Controles

É possível controlar a câmera através do teclado:

- Arrow Up = Subir câmera
- Arrow Down = Descer câmera
- Arrow Left / A = Girar câmera para esquerda
- Arrow Right / D = Girar cãmera para direita
- Q = Mover câmera para esquerda
- E = Mover câmera para direita


## Descrição da Implementação RESUMIDA

Alteramos a estrutura de nosso programa levemente, baseando-se em alguns dos projetos de exemplo alteramos nosso código para usar a classe model.
Temos as classes \`Balloon\`, \`Camera\`, \`Ground\`, \`Predio\` e \`Window\`, com as seguintes funções:
- \`Balloon\`: Classe que gera e movimenta o balão;
- \`Camera\`: Classe que gera e gerencia a câmera e seus movimentos;
- \`Ground\`: Classe que desenha e gera o chão;
- \`Predio\`: Classe que gera um prédio;
- \`Janela\`: Classe que gera janela;
- \`Window\`: Classe que gerencia a visão do usuário e interação entre o usuário e os gráficos apresentados.
- \`Model\`: Classe que renderiza os modelos 3D.

### \`Predio\`
Internalizamos os métodos de geração aleatória para o prédio, além de fazer com que essa classe seja responsável por chamar o shader e interagir com a classe model, que será responsável por gerar os modelos. Essa classe também é responsável por gerenciar a classe janela.
```cpp
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
  glm::vec4 lightPosWorldSpace{};   // Posição da luz
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
             float windowOffsetX, float windowOffsetZ);
  void update(glm::vec4 lightColorParam, glm::vec3 lightPosWorldSpaceParam);
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
```

### \`Janela\`
Essa nova classe é uma adaptação do novo código que fizemos para o prédio usado para gerar janelas. Usamos isso para separar cada objeto e cada um ter seu proprio shader e m_program, para tambem separar a textura de cada objeto.

### \`Model\`
A classe model representa a classe que renderizara todos os objetos 3D, e tambem é responsável por dar load nas texturas, pegamos de exemplo de um dos programas viewer das notas de aula.

### Shader do predio
O shader implementa um modelo de iluminação Blinn-Phong com mapeamento triplanar para texturas. A ideia básica é aplicar texturas de diferentes direções (X, Y e Z) em cada componente de um objeto 3D, proporcionando um mapeamento mais uniforme independentemente da orientação da superfície. Para o .frag, não houve mudanças para o apresentado em aula.

O trecho a seguir realiza a amostragem de textura triplanar, calcula uma cor ponderada pela normal e armazena o resultado em 'color'. Se o fragmento está na frente, 'color' é atribuído diretamente; caso contrário, é convertido para escala de cinza.

```glsl
// Triplanar mapping
vec3 offset = vec3(-0.5, -0.5, -0.5);
vec2 texCoord1 = PlanarMappingX(fragPObj + offset);
vec4 color1 = BlinnPhong(fragN, fragL, fragV, texCoord1);

vec2 texCoord2 = PlanarMappingY(fragPObj + offset);
vec4 color2 = BlinnPhong(fragN, fragL, fragV, texCoord2);

vec2 texCoord3 = PlanarMappingZ(fragPObj + offset);
vec4 color3 = BlinnPhong(fragN, fragL, fragV, texCoord3);

vec3 weight = abs(normalize(fragNObj));
color = color1 * weight.x + color2 * weight.y + color3 * weight.z;
```

No caso do .vert, tentamos fazer uma alteração para considerar a posição da luz ao invés da direção:

```cpp
#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightPosWorldSpace; // Alterado de lightDirWorldSpace para lightPosWorldSpace

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;
out vec3 fragPObj;
out vec3 fragNObj;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;

  // Calcula a direção da luz no espaço da visão
  vec3 lightPosViewSpace = (viewMatrix * lightPosWorldSpace).xyz; // Transforma a posição da luz para o espaço da visão
  vec3 L = lightPosViewSpace - P; // Direção da luz = posição da luz - posição do vértice

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;
  fragPObj = inPosition;
  fragNObj = inNormal;

  gl_Position = projMatrix * vec4(P, 1.0);
}
```

## Skybox adicionado
Foi adicionado um Skybox com textura cubica, o cubo foi criado proceduralmente, e a textura foi adicionada, foi baseada no viewer6 desenvolvido em sala de aula. A seguir mostra os metodos utilizados no \`Window.cpp\. Adicionamos um cubo do tamanho proporcional a cidade, que tambem foi aumentada em relacao ao SAMPA CITY 1.0.


#### Alterações na Classe \`Balloon\`
Os métodos adicionados são relacionados ao movimento do carro, sendo \`dolly(float speed)\` utilizado para o movimento para frente e para trás, \`truck(float speed)\` utilizado para o movimento para direita e esquerda e \`tilt(float speed)\` para o movimento para cima e para baixo. Esses métodos foram adaptados da classe \`Camera\`

A posição do balão foi definida como {0.0f, 3.0f, 0.0f} para que ele parta do centro da cidade de um ponto elevado e a direção foi definida {0.0f, 3.0f, -1.0f}, para que ele inicie se dirigindo a direção negativa do eixo z.

## Variáveis de Iluminação no Shader

As variáveis de iluminação na classe `Predio.hpp` foram definidas da seguinte forma:

```cpp
glm::vec4 lightColor{}; // Luz branca
glm::vec4 lightPos{};   // Posição da luz
glm::vec4 Ia{};          // Intensidade da luz ambiente
glm::vec4 Ka{};          // Coeficiente de reflexão ambiente
glm::vec4 Id{};          // Intensidade difusa
glm::vec4 Is{};          // Intensidade especular
glm::vec4 Kd{};          // Coeficiente de reflexão difusa
glm::vec4 Ks{};          // Coeficiente de reflexão especular
float shininess{};       // Brilho especular
```

### Parâmetros Iluminação Predio

No método \`Window::onPaintUI\` , um menu personalizado chamado "Parâmetros Iluminação Predio" foi adicionado. Esse menu inclui sliders que permitem ao usuário ajustar dinamicamente os valores dessas variáveis. A interatividade proporciona a flexibilidade de modificar a aparência dos prédios em tempo real, permitindo experimentação e ajustes visuais durante a execução do programa.

```cpp
// Dentro do método onPaintUI()
  {
    // Defina o tamanho e a posição da nova janela
    ImVec2 windowSize(300, 250);
    float newWindowX = 5;
    float newWindowY = 210;

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(newWindowX, newWindowY),
                            ImGuiCond_FirstUseEver);

    // Inicie uma nova janela ImGui para os parâmetros de iluminação do prédio
    ImGui::Begin("Parâmetros Iluminação Prédio");

    // Adicione sliders para ajustar os atributos de iluminação
    // Dentro do bloco de código para criar a nova janela no ImGui
    ImGui::SliderFloat("Cor da Luz (R)", &m_predio.lightColor.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Cor da Luz (G)", &m_predio.lightColor.y, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Cor da Luz (B)", &m_predio.lightColor.z, 0.0f, 1.0f, "%.2f");

    ImGui::SliderFloat("Posição da Luz x", &m_predio.lightPosWorldSpace.x, -10.0f, 10.0f,"%.2f");
    ImGui::SliderFloat("Posição da Luz y", &m_predio.lightPosWorldSpace.y, -20.0f, 0.0f,"%.2f");
    ImGui::SliderFloat("Posição da Luz z", &m_predio.lightPosWorldSpace.z, -10.0f, 10.0f,"%.2f");
    ImGui::SliderFloat("Intensidade Ambiente", &m_predio.Ia.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat4("Reflexão Ambiente", &m_predio.Ka.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Intensidade Difusa", &m_predio.Id.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Reflexão Difusa", &m_predio.Kd.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Intensidade Especular", &m_predio.Is.x, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Reflexão Especular", &m_predio.Ks.x, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Brilho Especular", &m_predio.shininess, 1.0f, 150.0f, "%.2f");

    // Finalize a janela ImGui
    ImGui::End();
  }
  ```
