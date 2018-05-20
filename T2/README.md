# T2: Implementação do Pipeline Gráfico
### `Grupo: Lucas Rincon, Thiago Henrique e Thiago Gonzaga`

Este post constitui o relatório sobre o segundo trabalho da disciplina de Introdução à Computação Gráfica, ministrada pelo Prof. Christian Pagot, no período 2017.2.

O trabalho consiste em implementar as transformações de um objeto tridimensional no pipeline gráfico, desde o espaço deste (do objeto) até o espaço de tela. O projeto deve ser implementado em liguagem C/C++ juntamente com as bibliotecas GLUT e OpenGL.

O objeto deve ser carregado a partir de um arquivo no formato .obj e ser mostrado na tela após as transformações do Pipeline. Para auxiliar-nos em tal tarefa, o professor disponibilizou um loader que arquivos .obj implementado em C++. O Loader interpreta o arquivo e devolve as faces do objeto juntamente com as coordenadas de seus vértices, possibilitando realizar transformações com estes.

Como o processo das transformações no pipeline gráfico utiliza constantemente matrizes, utilizará-se como auxílio a biblioteca [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page), que possui implementações de diversas variáveis matemáticas para C++, incluindo Matrizes.

Então neste relatório tentarei resumir os processos contidos no Pipeline Gráfico, que pode ser expresso neste diagrama de fluxo:

![pipeline](https://github.com/oitgg/href/blob/master/imgCGT2/1.png)

# Espaço do Objeto ⇒ Espaço do Universo

Todo objeto (em computação gráfica), seja ele bidimensional ou tridimensional, possui um eixo de coordenadas que mostra onde é a origem do objeto e como ele está orientado. Para este trabalho utilizaremos objetos 3d, ou seja, para descrever nossos objetos precisamos utilizar uma base ortonormal X, Y, Z.

Temos então o eixo X, o eixo Y e o eixo Z. Então, para posicionar objetos no universo, é necessário realizar transformações pois nem sempre queremos o objeto na origem do universo, que é o que aconteceria caso este não sofresse transformações e tiver sido modelado na origem.

Pode-se aplicar a um objeto transformações de **Escala**, **Rotação**, **Translação** e ***Shear***. Para isso utilizamos matrizes de transformação, pois estas facilitam a implementação, além de possibilitar realizar todas as transformações com apenas uma matriz (todas as matrizes de transformação podem ser multiplicadas para gerar uma única matriz de transformação do espaço do objeto para o espaço de tela, por exemplo).

# Escala
A Transformação de Escala altera as proporções do objeto, podendo ser **Isotrópica**, onde não há alteração do aspecto do objeto (a escala para todas as coordenadas do objeto é igual), ou **Anisotrópica**, onde a escala para pelo menos uma das coordenadas é diferente.

_Exemplos_:
### Escala Isotrópica
![isotropica](https://github.com/oitgg/href/blob/master/imgCGT2/3.png)
### Escala Anisotrópica:
![anisotropica](https://github.com/oitgg/href/blob/master/imgCGT2/4.png)

A matriz de Escala é dada deste modo:

![matriz](https://github.com/oitgg/href/blob/master/imgCGT2/5.png)

Onde sx é a escala no eixo **X**, sy é a escala no eixo **Y** e sz é a escala no eixo **Z**.

Notemos que para representar uma transformação tridimensional é necessário usar uma matriz 4x4. Isso ocorre devido a impossibilidade de representar translações num espaço tridimensional com matrizes. Para isso é necessário aumentar o grau do espaço em um, realizando as transformações em um espaço chamado de Homogêneo. Para ir para o espaço homogêneo a partir de uma matriz 3x3 (tridimensional) basta aumentar uma linha e uma coluna, adicionando uma coordenada Homogênea W = 1 na posição (4,4) da matriz gerada.

Os novos pontos transformados do objeto são obtidos multiplicando a matriz de transformação pelo vetor com as coordenadas do ponto:

![matriz](https://github.com/oitgg/href/blob/master/imgCGT2/6.png)

# Rotação
Rotações são efetuadas sobre eixos de coordenadas, ou seja, é possível realizar 3 rotações com um objeto tridimensional:

Matriz de rotação sobre X:
![matrizx](https://github.com/oitgg/href/blob/master/imgCGT2/7.png)

Matriz de rotação sobre Y:
![matrizy](https://github.com/oitgg/href/blob/master/imgCGT2/8.png)

Matriz de rotação sobre Z:
![matrizz](https://github.com/oitgg/href/blob/master/imgCGT2/9.png)

Onde θ é o ângulo da rotação.

Na rotação um eixo é escolhido para ser fixado, enquanto os outros dois giram em torno dele.

Para se obter o vetor resultante da transformação basta fazer o mesmo feito com a escala (multiplicar a matriz de rotação pelo vetor original). Entretanto, não se irá estender muito com transformações de espaço para espaço. A partir daqui o foco será em construir todas as matrizes de transformação, pois no final obteremos a matriz ModelViewPort, que transforma diretamente do espaço do objeto para o espaço de tela - e foi justamente desse modo que foi implementado no programa em C++.

# Translação
A matriz de translação vai simplesmente transladar os pontos que forem transformados pela matriz:
![matrizt](https://github.com/oitgg/href/blob/master/imgCGT2/10.png)

Onde dx é o deslocamento (translação) no eixo **X**, dy é o deslocamento em **Y** e dz é o deslocamento em **Z**.

# *Shear*
O shear já é uma transformação mais complicada, que não será explorada a fundo aqui, uma vez que este trabalho não implementou matriz de shear. Mas resumidamente o shear é uma transformação que distorce mais ou menos um ponto de acordo com a distância dele da origem: quanto mais longe da origem um ponto está, mais distorção ele terá, e vice-versa.

É possível tomar como exemplo um shear em 2D:

![shear](https://github.com/oitgg/href/blob/master/imgCGT2/11.png)

Exemplo de matriz de shear 3D:

![matrizs](https://github.com/oitgg/href/blob/master/imgCGT2/12.png)

Onde mx e my são shears em **X** e **Y**, respectivamente, ao longo do eixo **Z**.

# Matriz Model
Com todas essas transformações podemos compor a matriz Model, que transforma pontos no espaço do objeto para o espaço do universo. A Matriz Model é o produto de todas as matrizes de transformação do objeto, entretanto, deve-se tomar cuidado pois a ordem do produto de matrizes altera o resultado.

Segue um exemplo em 2D:
Seja **T** uma matriz de translação 2D e **R** uma de rotação.
Aplicando a **Rotação** primeiro, temos a matriz:
```sh
M1 = T * R
```
E aplicando a **Translação** primeiro, temos:
```sh
M2 = R * T
```

Aplicando as transformações M1 e M2 num mesmo objeto centrado na origem resultará nestas transformações:
![transformações](https://github.com/oitgg/href/blob/master/imgCGT2/13.png)

Perceba-se que as duas transformações resultaram em situações diferentes. É necessário prestar atenção à ordem da multiplicação das matrizes. Neste caso as primeiras transformações desejadas terão que ser os últimos termos na multiplicação. Então, levando em conta o exemplo anterior, caso se quisesse aplicar a rotação primeiro, a matriz **M1** seria a mais adequada. Geralmente, em composição de matrizes de transformação, é desejável que a translação seja uma das últimas transformações a serem aplicadas, logo, estas geralmente vêm no começo.

Para este programa, utiliza-se duas transformações apenas na matriz *Model*: Escala (por 1, não modifica o aspecto do objeto) e uma rotação (necessária para o trabalho).
```sh
Model = Model_S * Model_R
```

# Espaço do Universo ⇒ Espaço de Câmera
Na transformação do Espaço do Universo para o espaço da câmera os pontos serão transformados para o ponto de vista da câmera. Para isso se monta a matriz View.

# Matriz *View*
A Matriz *View* pode ser facilmente montada com os parâmetros da câmera: a posição da câmera; o vetor *LookAt* (aponta para onde a câmera está olhando); e o vetor *Up* (aponta para cima, para impedir que a câmera realize movimentos laterais, “cair para os lados”).

Primeiro é montada a base da camêra com esses parâmetros:

![basecam](https://github.com/oitgg/href/blob/master/imgCGT2/14.png)

Para fazer a transformação da base do espaço do Universo para a Base do espaço da Câmera, de acordo com estudos em álgebra linear, basta multiplicar a os pontos na base inicial pela inversa da base final. E como a base da câmera é ortonormal, sua inversa é sua transposta. É possível montar então:

![baset](https://github.com/oitgg/href/blob/master/imgCGT2/15.png)

E aplicamos a translação da câmera:

![trans](https://github.com/oitgg/href/blob/master/imgCGT2/16.png)

Logo,
```sh
View = Bt * T
```

# Espaço de Câmera ⇒ Espaço de Recorte
É no Espaço de Recorte que se adiciona distorção de perspectiva através da Matriz Projection. Na realidade a perspectiva é uma ilusão criada pela matriz *Projection* fazendo com que objetos mais próximos da câmera pareçam maiores e objetos mais distantes pareçam menores. É também no espaço de recorte que a parte visível do cenário é filtrada, entretanto, como este se trata de um programa mais simples e objetivo do que prático, tal limite não é estabelecido. No OpenGl os limites do espaço de recorte são definidos por um volume denominado View Frustum.

## Matriz Projection
Resumidamente, para criar distorção de perspectiva colocamos um *View Plane* a uma certa distância do centro de projeção da câmera. Chamaremos essa distância de variável d. Quanto menor esta variável d maiores os objetos mais próximos do *View Plane* e menores os mais longes.
![viewplane](https://github.com/oitgg/href/blob/master/imgCGT2/17.png)

A Matriz *Projection*, junto com o deslocamento do centro de projeção tem esse molde:

![matrizp](https://github.com/oitgg/href/blob/master/imgCGT2/18.png)

# Espaço de Recorte ⇒ Espaço Canônico
Para casos gerais, o espaço canônico define um cubo de (-1,-1,-1) até (1,1,1) que contêm toda a cena e homogeneíza os vetores transformados do espaço de Recorte. No OpenGl é feito desse modo, mas no caso deste relatório, por se tratar de um pipeline simples, o cubo não é definido, mas ainda é feita a homogeneização dos pontos que vieram do espaço de recorte, dividindo cada um por sua coordenada homogênea (que é provavelmente diferente de 1);

# Espaço Canônico ⇒ Espaço de Tela
Os pontos transformados no espaço canônico são transformados para o espaço de tela através da Matriz Port, que contém escalas e translações.

# Jump: Espaço do Objeto ⇒ Espaço de Tela
Como mencionado anteriormente, é possível aglomerar todas as Transformações em uma matriz só. É com essa ideia que foi feita a matriz **ModelViewPort**. Levando em consideração que a primeira transformação é a última matriz na multiplicação, pode-se:

```sh
Canonical = (Projection * View * Model)/W
```

```sh
ModelViewPort = Port * Canonical
```

# Implementação do Pipeline Gráfico
Primeiramente para a organização da workspace, o código do trabalho anterior, juntamente com o código-fonte do ObjLoader, foram copiados para um novo projeto na IDE, e foram realizadas algumas alterações para que o programa fosse capaz de ler arquivos .obj, e armazenar matrizes e vetores com e sem coordenada Homogênea (com matrizes da biblioteca Eigen). Assim poderia pegar as faces do .obj uma a uma (pelo ObjLoader) e criar um objeto da classe Triangle (implementada anteriormente) para cada, facilitando as transformações.

Para a matriz **Model** definiu-se uma escala com fator 1 para todas as coordenadas (mantendo a proporção do objeto) e uma rotação em torno de Y (com uma ângulo phi que é incrementado toda vez que o pipeline é executado, rotacionando o objeto).

Para a **câmera** definiu-se sua posição para (0,0,2), seu *lookAt* para (0,0,0) e seu *up* para (0,1,0). Alguns desses valores foram retirados do programa de comparação. A matriz *View* é gerada por esses valores dados.

Para o **espaço projetivo** (ou de **recorte**), definiu-se a variável d (distância do near plane) para 1 inicialmente. Ao realizar alguns testes, se descobriu que adotando 1.9 aproximava mais o resultado final do pipeline com o de comparação. Por fim o valor de d foi deixado como 1.9.

Comparando o programa original (à esquerda) com o meu pipeline (à direita) podemos observar a diferença com d = 1:
![d1](https://github.com/oitgg/href/blob/master/imgCGT2/19.png)

E com d = 1.9:
![d1.9](https://github.com/oitgg/href/blob/master/imgCGT2/20.png)

Para o **espaço canônico** a transformação é homogeneizada e na transformação para o espaço de tela ocorre primeiro uma escala para espelhar o eixo Y (pois na tela o eixo Y é para baixo), uma translação para mover os objetos para o quadrante da tela e por último outra escala para fazer os objetos se adaptarem ao tamanho da tela. Os pontos e linhas são pintados logo após esse estágio.


# Referências Bibliográficas
- Notas de Aula do Prof. Christian
- [Graphics pipeline, no Wikipedia](https://en.wikipedia.org/wiki/Graphics_pipeline)
- [Engen 3 Documentation (em inglês)](http://eigen.tuxfamily.org/dox/)

