# Sampa City 2 - A cidade agora é outra! 

O **Sampa City 2** é um projeto em 3D que simula a construção de uma cidade com prédios representados por cubos, ela é a evoluçao do Sampa City 1.0. A cidade é gerada de forma procedural, podendo ser alterada com base em uma semente específica. Os prédios são texturizado. Além disso, há a opção de clicar em um botão de aleatorização, que troca automaticamente as sementes, alterando as posições e tamanhos dos prédios. É possível definir o número de prédios que aparecem em cada quadrante. Além disso, podemos controlar as janelas dos prédios, mudando largura, profundidade e offset da janela, além de alterar se a mesma está ligada ou desligada. Há também um balão, que se move ao redor da cidade, podendo ser esse movimento horário ou anti-horário.

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

Temos as classes \`Balloon\`, \`Camera\`, \`Ground\`, \`Predio\` e \`Window\`, com as seguintes funções:
- \`Balloon\`: Classe que gera e movimenta o balão;
- \`Camera\`: Classe que gera e gerencia a câmera e seus movimentos;
- \`Ground\`: Classe que desenha e gera o chão;
- \`Predio\`: Classe que gera um prédio;
- \`Janela\`: Classe que gera janela;
- \`Window\`: Classe que gerencia a visão do usuário e interação entre o usuário e os gráficos apresentados.
- \`Model\`: Classe que renderiza os modelos 3D.

### \`Janela\`
Essa classe uma classe parecida com o do predio usado para gerar janelas. Usamos isso para separar cada objeto e cada um ter seu proprio shader e m_program, para tambem separar a textura de cada objeto.

### \`Model\`
A classe model representa a classe que renderizara todos os objetos 3D, e tambem e responsavel por dar load nas texturas, pegamos de exemplo de um dos programas viewer das notas de aula.

### Shader do predio
O shader implementa um modelo de iluminação Blinn-Phong com mapeamento triplanar para texturas. A ideia básica é aplicar texturas de diferentes direções (X, Y e Z) em cada componente de um objeto 3D, proporcionando um mapeamento mais uniforme independentemente da orientação da superfície.

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