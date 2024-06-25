# Documentação do Código exemplo_00.c

Este código é um exemplo básico de como criar uma janela e renderizar um triângulo colorido usando a biblioteca GLFW e a API OpenGL. 

## Inclusões de Bibliotecas

```c
#include <GL/glew.h>    // inclui lib auxiliar GLEW e a versão mais recente da OpenGL
#include <GLFW/glfw3.h> // GLFW biblioteca para interface com SO (janela, mouse, teclado, ...)
#include <stdio.h>      // biblioteca padrão C para I/O
```

As três primeiras linhas incluem as bibliotecas necessárias. GLEW é uma biblioteca que ajuda a gerenciar extensões OpenGL. GLFW é uma biblioteca para criar janelas e manipular eventos de entrada, como teclado e mouse. A biblioteca stdio é usada para operações de entrada e saída.

## Inicialização da GLFW

```c
if (!glfwInit())
{
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
}
```

Aqui, a biblioteca GLFW é inicializada. Se a inicialização falhar, uma mensagem de erro é impressa e o programa é encerrado.

## Configuração da Janela

```c
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

Essas linhas configuram a versão do OpenGL e o perfil que queremos usar. Neste caso, estamos usando a versão 3.2 do OpenGL no perfil core.

## Criação da Janela

```c
GLFWwindow *window = glfwCreateWindow(640, 480, "Versao do Renderer", NULL, NULL);
if (!window)
{
    fprintf(stderr, "*** ERRO: não foi possível abrir janela com a GLFW\n");
    glfwTerminate();
    return 1;
}
glfwMakeContextCurrent(window);
```

Aqui, uma janela é criada usando a função `glfwCreateWindow`. Se a criação da janela falhar, uma mensagem de erro é impressa, a biblioteca GLFW é terminada e o programa é encerrado. Se a janela for criada com sucesso, ela é feita para ser o contexto atual do OpenGL.

## Renderização do Triângulo

O restante do código é responsável pela criação e renderização de um triângulo colorido. Isso envolve a criação de buffers de vértices (VBOs) para armazenar os vértices do triângulo e suas cores, a criação de um objeto de array de vértices (VAO) para armazenar o layout dos dados do vértice, a criação de shaders para controlar como os vértices são transformados e coloridos, e finalmente desenhar o triângulo e trocar os buffers para exibir o resultado na janela.

## Finalização

```c
glfwTerminate();
return 0;
```

No final, a biblioteca GLFW é terminada e o programa é encerrado com sucesso.