# T1: Rasterização de Pontos e Linhas

### Grupo: Lucas Rincon, Thiago Henrique e Thiago Gonzaga

Este post constitui o relatório sobre o primeiro trabalho da disciplina de Introdução à Computação Gráfica, ministrada pelo Prof. Dr. Christian Pagot, no período 2017.2.

O trabalho consiste em implementar algoritmos de rasterização para pontos e linhas em liguagem C/C++ juntamente com as bibliotecas GLUT e OpenGL utilizando o framework disponibilizado previamente.

### Displays, Píxels e Memória

Os Monitores (displays) atuais são compostos por uma malha de pixels, com x pixels de largura por y pixels de altura, com um padrão RGB, ou seja, cada pixel é formado por 3 células, cada uma representando uma cor (R para vermelho (Red), G para verde (Green) e B para azul (Blue)). Com essas 3 cores é possível representar uma vasta gama de cores visíveis ao olho humano. A combinação das três as cores, em valor máximo, forma o branco, enquanto a combinação das três cores, em valor mínimo, forma o preto.

Para representar tais cores em um computador, geralmente são utilizados 8 bits para cada componente (canal) do pixel, dando 256 níveis de intensidade para cada cor. Combinando os 3 canais é possível representar aproximadamente 16 milhões de cores com um computador. Adicionalmente, existe um canal A (alpha), referente à transparência do pixel. Neste trabalho utilizaremos o espaço RGBA.

Vimos que pixels são compostos por 4 canais, cada um com 8 bits de memória (utilizaremos unsigned char para representar um canal), totalizando 32 bits de informação cada pixel. Para "pintar" os píxels na tela, escreveremos na memória de vídeo os valores de cada pixel. Entretanto, nos computadores modernos, o acesso à memória de vídeo é restrito, e não é possível alterá-los facilmente. Por isso foi concedido o Framework para tal trabalho. Nele existe o ponteiro FBptr (Frame Buffer Pointer), que aponta para a posição em memória referente ao pixel (0,0) na tela.

### Armazenamento de píxels

O nossa malha de tela é constitui um espaço bidimensional, porém a memória é linear. Assim, é necessário calcular um offset para que cada pixel da tela, dada sua posição (x,y) ocupe um único e distinto espaço em memória.

Para tanto, foi apresentado um algoritmo em sala de aula que efetua tal alocamento, como pode ser exemplificado na imagem apresentada:

![display](https://github.com/oitgg/href/blob/master/imgCG/1.png)

Levando em consideração que a o número de pixels na tela é dado pela multiplicação de número de linhas * número de colunas, a memória deve ter esta mesma quantidade de posições para representar a tela, só que de forma linear.

Para isso foi-nos apresentado tal algoritmo, que decide a posição de cada pixel e cada canal de um pixel na memória, considerando largura em pixels da tela:

![pixel](https://github.com/oitgg/href/blob/master/imgCG/2.png)
Onde w é a largura em pixels da tela.

Para alocar o framebuffer e inicializar suas posições com 0, implementou-se a função InitDataStructures():

```sh
FBptr = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT * 5];
for (unsigned int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT ; i++)
{
	FBptr[i*4]   = 0;
	FBptr[i*4+1] = 0;
	FBptr[i*4+2] = 0;
	FBptr[i*4+3] = 255;
}
```

E para desenhar cada um dos 4 canais de cor do ponto desejado, segui-se a seguinte lógica:

```sh
FBptr[4*y*IMAGE_WIDTH + 4*x]      = rgba[0];
FBptr[4*y*IMAGE_WIDTH + 4*x +1]   = rgba[1];
FBptr[4*y*IMAGE_WIDTH + 4*x +2]   = rgba[2];
FBptr[4*y*IMAGE_WIDTH + 4*x +3]   = rgba[3];
 ```
 
 Por exemplo, se compilar-se:
```sh
PutPixel(256,106,cor0);
PutPixel(356,206,cor1);
PutPixel(133,306,cor2);
PutPixel(156,432,cor1);
PutPixel(73,233,cor0);
PutPixel(189,512,cor2);
PutPixel(133,37,cor1);
```
Obtém-se:
![exemplopixel](https://github.com/oitgg/href/blob/master/imgCG/exemplopixels.png)
 

### Rasterização de Retas

A rasterização de uma reta é a discretização de um modelo matemático para uma matriz bidimensional. 

![matriz](https://github.com/oitgg/href/blob/master/imgCG/3.png)

Basicamente, é a representação de uma reta, formada por dois vértices, na tela do computador. Como trata-se de um processo altamente custoso por utilizar muitos floats e multiplicações, temos de utilizar um algoritmo com abordagem aproximativa, como por exemplo, o algoritmo de Bresenham.

O Algorítimo de Bresenham utiliza a equação geral da reta para decidir qual ponto no eixo y deve ser pintado.
```sh
a*x + b*y + c = 0
```
Que pode também representar um plano:
```sh
f(x,y) = a*x + b*y + c
```
Comparando com a equação da reta, temos:
```sh
y = (Δy/Δx)*x + c
=> Δy*x - Δx*y + Δx*C = 0
```
Assim:
```sh
a = Δy, b = Δx, c = Δx*c
```

Utilizando uma das exemplificações dadas em aula, sabemos que pintaremos os pixels da esquerda para a direita, então o próximo pixel a ser pintado pode ser (x+1,y) ou (x+1,y+1). Para decidir, é calculado um “erro de aproximação” aplicando a equação do plano no ponto médio entre os pixels que podem ser escolhidos:

![pos](https://github.com/oitgg/href/blob/master/imgCG/4.png)

Se a equação retornar um valor positivo, isso significa dizer que a reta passa por baixo do ponto, ou seja, está mais próxima do ponto (x+1,y), portanto este deve ser pintado. Caso o valor seja negativo, significa o contrário, e que o pixel a ser pintado é o (x+1,y+1).

O algorítmo utilizado para o cálculo das posições x e y do ponto em relação à referência de orientação, e da aproximação e correção do erro em relação ao eixo de orientação é o seguinte:
```sh
if(dx > dy) {
x += pX;
p += dy;

if(p >= dx) 
{
y += pY;
p -= dx;
}
            } 
else {
y += pY;
p += dx;

if(p >= dy) 
{
x += pX;
p -= dy;
}
     }
```

![octantes](https://github.com/oitgg/href/blob/master/imgCG/exemplooctantes.png)

Com isso, uma função para o desenho das linhas, que recebe as posições iniciais e finais dos dois pontos, juntamente com sua cor (interpolada ou não) foi implementada: 
```sh
DrawLine(x0, y0, x1, y1, cor0, cor1)
```

Caso compile-se:
```sh
DrawLine(180,180,380,380,cor0,cor0);
```
Obtém-se:
![reta](https://github.com/oitgg/href/blob/master/imgCG/exemploreta.png)


### Interpolação Linear

O Algoritmo de Interpolação Linear de Cores consiste em interpolar as cores entre o ponto inicial e final da linha de forma linear, ou seja, deve-se ter um gradiente/degradê suave entre os dois pontos da linha.

Pensando assim, realiza-se a diferença de cores entre os pontos final e inicial da linha, obtendo uma variação de cor. Dependendo do octante, divide-se esta variação de cor pelo maior deslocamento da reta num eixo (por exemplo, Δx nos octantes 1 e 8), e isso nos dá um valor que se pode incrementar na cor inicial a cada incremteno no eixo x ou no eixo y a depender do caso.

Para realizar a interpolação das cores, primeiramente calcula-se a distância média entre ambos os pontos (x0,y0) e (x1,y1), a fim de aproximar o número de pontos na reta:
```sh
dTotal = sqrt((dx*dx) + (dy*dy));
```
Depois compara-se os valores de RGB para os pontos final e inicial:
```sh
compR = rgbaP1[0] - rgbaP0[0];
compG = rgbaP1[1] - rgbaP0[1];
compB = rgbaP1[2] - rgbaP0[2];
compA = rgbaP1[3] - rgbaP0[3];
```
Com os valores obtidos nas comparações, calcula-se os incrementos em cima da cor a serem feitos por ponto:
```sh
iR = compR / dTotal;
iG = compG / dTotal;
iB = compB / dTotal;
iA = compA / dTotal;
```
Define-se também os valores iniciais para cada cor:
```sh
r = rgbaP0[0];
g = rgbaP0[1];
b = rgbaP0[2];
a = rgbaP0[3];
```
E finalmente, para a interpolação, ocorre a atualização do valor da cor para o ponto atual
```sh
r += iR;
g += iG;
b += iB;
a += iA;
```

Caso compile-se:
```sh
DrawLine(180,180,380,380,cor1,cor0);
DrawLine(306, 206, 256, 106,cor0,cor1);
DrawLine(452, 230, 512, 94,cor1,cor2);
DrawLine(379, 334, 89, 76,cor2,cor1);
DrawLine(238, 493, 177, 176,cor0,cor2);
DrawLine(507, 374, 93, 433,cor2,cor0);
```
Obtém-se as seguintes retas:

![interpolação](https://github.com/oitgg/href/blob/master/imgCG/exemplointerp.png)


### Rasterização de Triângulos

Após a parte mais trabalhosa em termos de implementação, foi desenvolvida a função DrawTriangle(). Esta função desenha as arestas de um triângulo na tela recebendo como parâmetros os três pixels que representam os vértices. Lembrando que cada objeto pixel possui as informações de suas coordenadas x e y de cada vértice, além de três componentes RGBA, para otimizar a interpolação das cores entre os três pontos.

```sh
void DrawTriangle(int xa, int ya, int xb, int yb, int xc, int yc,
                  int rgbaP0[], int rgbaP1[], int rgbaP2[]) 
{
DrawLine(xa, ya, xb, yb, rgbaP0, rgbaP1);
DrawLine(xb, yb, xc, yc, rgbaP1, rgbaP2);
DrawLine(xc, yc, xa, ya, rgbaP2, rgbaP0);
}
```

Compilando o seguinte exemplo:
```sh
DrawTriangle(106, 406, 256, 106, 406, 406, cor0, cor1, cor2);
DrawTriangle(83, 256, 512, 83, 256, 512, cor1, cor2, cor0);
```
Obtém-se:
![triangulos](https://github.com/oitgg/href/blob/master/imgCG/exemplotriang.png)


Estes algoritimos são realmente eficazes para rasterização e interpolação de retas e triângulos, assim como os testes comprovam, e ainda é possível realizar diversas outras aplicações com estes algorítmos para criar formas diferentes (como o círculo rasterizado com linhas), com auxílio de funções matemáticas.

### Considerações finais e dificuldades

As maiores dificuldade encontrada neste trabalho foram a generalização do algoritmo de Bresenham para todos os octantes e a implementação do algorítmo de interpolação, porém após várias sessões de debug intenso, os erros foram contornados e as dificuldades foram superadas.

Foi realizado também, a tentativa de implementação do preenchimento por completo dos triângulos, mas a deadline não permitiu tal feito. E foi optado deixar essa implementação para uma melhoria futura em uma outra ocasião.

## Referências Bibliográficas

• Notas de Aula do Prof. Christian
• [Rasterização em C/C++ (Algorítmo de Bresenham)](https://bitunico.wordpress.com/2012/12/16/rasterizacao-em-cc-algoritmo-de-bresenham/)
• [The Bresenham Line-Drawing Algorithm](https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html)
• [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
