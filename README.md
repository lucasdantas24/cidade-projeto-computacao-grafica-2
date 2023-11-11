# Sampa City - Projeto em 3D

O **Sampa City** é um projeto em 3D que simula a construção de uma cidade com prédios representados por cubos. A cidade é gerada de forma procedural, podendo ser alterada com base em uma semente específica. Os prédios são coloridos e mudam de cor dinamicamente. Além disso, há a opção de clicar em um botão de aleatorização, que troca automaticamente as sementes, alterando as posições e tamanhos dos prédios.

# Link WEB
O link web se encontra aqui


## Classes Principais

### \`Window.cpp\`

Este arquivo contém a implementação da classe \`Window\`, que é responsável por gerenciar a janela de renderização e interação com o usuário. Destacarei alguns métodos relevantes desta classe:

#### \`onEvent(SDL_Event const &event)\`

Este método trata eventos de teclado, ajustando velocidades de movimento com base nas teclas pressionadas.

#### \`gerarAndaresPorPredio(int num_building, int seed)\`

Gera aleatoriamente o número de andares para cada prédio com base em uma semente.

#### \`gerarLarguraProfundidadeAleatorio(int num_building, int seed)\`

Gera aleatoriamente valores para largura e profundidade dos prédios com base em uma semente.

#### \`gerarCoresAleatorias(int numBuildings)\`

Gera cores aleatórias para os prédios.

#### \`generateRandomBuildingPositions(int numBuildings, int seed, float a, float b, float c, float d)\`

Gera posições aleatórias para os prédios dentro de uma área específica, evitando sobreposições.

#### \`isPositionValid(const std::vector<glm::vec3> &positions, const glm::vec3 &newPosition, float radius)\`

Verifica se uma nova posição é válida, evitando sobreposições.

#### \`onCreate()\`

Configurações iniciais, criação de shaders, e inicialização de buffers e objetos.

#### \`loadModelFromFile(std::string_view path)\`

Carrega modelos 3D de um arquivo.

#### \`onPaint()\`

Renderiza a cidade, ajustando posições, tamanhos e cores dos prédios. Também renderiza um carro e o solo.

#### \`onPaintUI()\`

Renderiza a interface do usuário, incluindo um slider para ajustar a semente e uma opção de aleatorização.

#### \`onResize(glm::ivec2 const &size)\`

Atualiza a matriz de projeção da câmera ao redimensionar a janela.

#### \`onDestroy()\`

Limpeza de recursos OpenGL ao fechar a aplicação.

#### \`onUpdate()\`

Atualizações lógicas, incluindo movimento do carro.

#### \`updateCar(float deltaTime)\`

Atualiza a posição do carro.

#### \`isAtCrossRoad(glm::vec3 position)\`

Verifica se o carro está em um cruzamento.

## Outras Classes

Além da classe \`Window\`, o projeto inclui outras classes como \`Camera\`, \`Car\`, \`Ground\`, e \`Predio\`, responsáveis por funcionalidades específicas.

 Em seguida, compile e execute o código-fonte.


