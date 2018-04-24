## PortalGL

Esta é uma réplica da engine do jogo Portal escrito em OpenGL.


## Dependencias

* Opengl, Glut FreeImage e CMake são as principais dependências necessárias para buildar esse projeto;
* Todas as dependências são facilmente instaladas a partir do package manager do seu OS;
* Para facilitar, recomenda-se executar o seguinte comando no seu terminal:
```
sudo apt install cmake libfreeimage-dev freeglut3-dev mesa-utils libxmu-dev libxi-dev
```

* Para a realização de alguns cálculos intrínsecos da lógica matemática do projeto, utiliza-se a biblioteca CML (Configurable Math Library);
* A CML é uma biblioteca header-only para lidar com algumas operações matemáticas, e está setado como um submódulo desse repositório, para ativá-lo e adicionar seus arquivos no seu repositório local, execute:
```
    git submodule init
    git submodule update
```

## Instruções de Build

Crie o diretório `build` e vá até o mesmo:

```
mkdir build
cd build
```

Como algumas operações são dependentes do GCC, é necessário o set da flag do padrão C++11, para isto, no diretório raíz do projeto, procure o arquivo chamado `CMakeLists.txt` e abra-o com o editor de texto de sua escolha
Na segunda linha, abaixo de `cmake_minimum_required(VERSION 2.8)`, insira a seguinte flag:
```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
```
Salve e feche o `CMakeLists.txt`

A partir do diretório `build` chame as funções, `cmake` e `make`

```
cmake ..
make
```

Muito provavelmente, seu compilador acusará vários warnings e a compilação irá falhar. (Causa ainda não identificada, muito provavelmente está correlacionado ao GCC/G++)
Caso isso aconteça, faça o seguinte:
* Vá até o arquivo `CMakeLists.txt`, remova a flag anteriormente adicionada, e refaça o `make`. Ainda aparecerão erros de compilação;
* Vá novamente até o arquivo `CMakeLists.txt`, adicione a flag novamente, e refaça o `make`.

Caso o `make` seja completo com sucesso, o executável será criado no diretório `build`.
Para executar o jogo, apenas entre com o comando `./portalgl` (Em algumas máquinas, é possível a execução do mesmo a partir do ícone do executável, no diretório).
