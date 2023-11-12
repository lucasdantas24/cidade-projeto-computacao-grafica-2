# Sampa City - Projeto em 3D

O **Sampa City** é um projeto em 3D que simula a construção de uma cidade com prédios representados por cubos. A cidade é gerada de forma procedural, podendo ser alterada com base em uma semente específica. Os prédios são coloridos, tendo a opção de gerar cores aleatoriamente ou definir uma única cor para todos os prédios. Além disso, há a opção de clicar em um botão de aleatorização, que troca automaticamente as sementes, alterando as posições e tamanhos dos prédios. É possível definir o número de prédios que aparecem em cada quadrante. Além disso, podemos controlar as janelas dos prédios, mudando largura, profundidade e offset da janela, além de alterar se a mesma está ligada ou desligada. Há também um balão, que se move ao redor da cidade, podendo ser esse movimento horário ou anti-horário.

# Link WEB
O link web se encontra [aqui](https://rodrigohisashi.github.io/cidade-projeto-computacao-grafica/sampa/).

# Controles

É possível controlar a câmera através do teclado:

- Arrow Up = Subir câmera
- Arrow Down = Descer câmera
- Arrow Left / A = Girar câmera para esquerda
- Arrow Right / D = Girar cãmera para direita
- Q = Mover câmera para esquerda
- E = Mover câmera para direita


# Descrição da Implementação

Temos as classes \`Balloon\`, \`Window\`, \`Window\`



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

## Outras Classes

Além da classe \`Window\`, o projeto inclui outras classes como \`Camera\`, \`Car\`, \`Ground\`, e \`Predio\`, responsáveis por funcionalidades específicas.

 Em seguida, compile e execute o código-fonte.


