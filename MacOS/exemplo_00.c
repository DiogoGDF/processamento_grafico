#include <GL/glew.h>    // inclui lib auxiliar GLEW e a versão mais recente da OpenGL
#include <GLFW/glfw3.h> // GLFW biblioteca para interface com SO (janela, mouse, teclado, ...)
#include <stdio.h>      // biblioteca padrão C para I/O

int main()
{
    // 1 - Inicialização da GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    // 1.1 - Necessário para Apple OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2 - Criação do contexto gráfico (window)
    // Código de criação de janela, dimensionamento e título
    GLFWwindow *window = glfwCreateWindow(640, 480, "Versao do Renderer", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "*** ERRO: não foi possível abrir janela com a GLFW\n");
        // 2.0 - Se não foi possível iniciar GLFW, então termina / remove lib GLFW da memória.
        glfwTerminate();
        return 1;
    }
    // 2.1 - Torna janela atual como o contexto gráfico atual para desenho
    glfwMakeContextCurrent(window);

    // 3 - Inicia manipulador da extensão GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // 4 - Objetivo do exemplo: detectar a versão atual do OpenGL e motor de renderização.
    // Obtenção do nome do motor de renderização e da versão da OpenGL
    const GLubyte *renderer = glGetString(GL_RENDERER);
    // Obtenção da versão da OpenGL
    const GLubyte *version = glGetString(GL_VERSION);
    printf("\n** Verificação da OpenGL ************\n");
    printf("\tRenderer: %s\n", renderer);
    printf("\tVersão suportada da OpenGL %s\n\n", version);

    /**************
     * 5 - AQUI VEM TODO O CÓDIGO DA APP GRÁFICA, PRINCIPALMENTE, O MAIN LOOP
     **************/

    // 5.1 - Geometria da cena
    // VBO
    // Definição de um array de pontos que representam os vértices de um triângulo
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f
    };
    // declaração de uma variável para armazenar o identificador do VBO
    GLuint vbo = 0;
    // gera o identificador do VBO e armazena em vbo
    glGenBuffers(1, &vbo);
    // Vincula o VBO para que as operações subsequentes afetem esse buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Copia os dados do array points para o VBO, o quarto parametro é um ponteiro para os dados. GL_STATIC_DRAW é um tipo de uso do buffer, que indica que os dados não serão alterados.
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    // VBO Colors
    // Definição de um array de pontos que representam as cores dos vértices de um triângulo. Cada cor é representada por 3 floats (RGB).
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 1.0f
    };
    // declaração de uma variável para armazenar o identificador do VBO
    GLuint vboColors = 0;
    // gera o identificador do VBO e armazena em vbo
    glGenBuffers(1, &vboColors);
    // Vincula o VBO para que as operações subsequentes afetem esse buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    // Copia os dados do array points para o VBO, o quarto parametro é um ponteiro para os dados. GL_STATIC_DRAW é um tipo de uso do buffer, que indica que os dados não serão alterados.
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    // VAO
    // declaração de uma variável para armazenar o identificador do VAO (Vertex Array Object)
    GLuint vao = 0;
    // gera o identificador do VAO e armazena em vao
    glGenVertexArrays(1, &vao);
    // Vincula o VAO para que as operações subsequentes afetem esse buffer
    glBindVertexArray(vao);
    // Vincula o VBO ao VAO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Habilita o atributo de vértice. (O primeiro parâmetro é o índice do atributo de vértice. O índice é o mesmo que o layout no shader. O segundo parâmetro é o tamanho do atributo de vértice. O terceiro parâmetro é o tipo de dado do atributo de vértice. O quarto parâmetro é se o valor deve ser normalizado. O quinto parâmetro é o tamanho do vértice. O sexto parâmetro é o deslocamento do vértice.)
    glEnableVertexAttribArray(0);

    // Define os dados do atributo de vértice. O índice 0 é o índice do atributo de vértice. O tamanho do atributo de vértice é 3. O tipo de dado do atributo de vértice é float. O valor não é normalizado. O tamanho do vértice é 0. O deslocamento do vértice é NULL.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // 5.2 - Objeto de propriedades / layout
    // VAO Colors
    // Vincula o VBO de cores ao VAO
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    // Habilita o atributo de cor.
    glEnableVertexAttribArray(1);
    // Define os dados do atributo de cor.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // 5.3 - Shaders
    // 5.3.1 - Vertex shader
    // O shader de vértices é responsável por transformar os vértices de um objeto 3D em coordenadas 2D e também por passar as cores dos vértices para o shader de fragmentos. 
    const char* vertex_shader =
        "#version 400\n"
        // layout(location=0) indica que o atributo de vértice está no layout 0. in vec3 vp; indica que o atributo de vértice é um vec3 (vetor de 3 floats) chamado vp.
        "layout(location=0) in vec3 vp;" //vp: um vértice do VBO de vértices
        // layout(location=1) indica que o atributo de cor está no layout 1. in vec3 vc; indica que o atributo de cor é um vec3 (vetor de 3 floats) chamado vc.
        "layout(location=1) in vec3 vc;" //vc: um vértice de cor do VBO de cores
        // out vec3 color; indica que a cor do vértice será passada para o shader de fragmentos.
        "out vec3 color;"
        // função principal do shader de vértices. Ela define a posição do vértice e a cor do vértice.
        "void main () {"
        // define a posição do vértice. vp é a posição do vértice. 1.0 é a coordenada w do vértice.
        "  gl_Position = vec4 (vp, 1.0);"
        // define a cor do vértice. vc é a cor do vértice.
        "  color = vc;"
        "}"
    ;

    // 5.3.2 - Fragment shader
    // O shader de fragmentos é responsável por calcular a cor de cada pixel na tela.
    const char* fragment_shader =
        // versão do shader
        "#version 400\n"
        // indica que a entrada da cor do vértice.
        "in vec3 color;"
        // indica que a cor do fragmento é um vec4 (vetor de 4 floats) chamado frag_color.
        "out vec4 frag_color;"
        "void main () {"
        // define a cor do fragmento. color é a cor do vértice.
        "  frag_color = vec4 (color, 1.0);"
        "}"
    ;

    // 5.4 - Compilação e vinculação dos shaders
    // Cria um shader de vértices, o compila e retorna seu identificador.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    // Especifica o código fonte do shader de vértices.
    glShaderSource(vs, 1, &vertex_shader, NULL);
    // Compila o shader de vértices.
    glCompileShader(vs);

    // Cria um shader de fragmentos, o compila e retorna seu identificador.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    // Especifica o código fonte do shader de fragmentos.
    glShaderSource(fs, 1, &fragment_shader, NULL);
    // Compila o shader de fragmentos.
    glCompileShader(fs);

    // Cria um programa de shader, o anexa ao shader de vértices e de fragmentos, o vincula e retorna seu identificador.
    GLuint shader_program = glCreateProgram();
    // Anexa o shader de vértices ao programa de shader.
    glAttachShader(shader_program, fs);
    // Anexa o shader de fragmentos ao programa de shader.
    glAttachShader(shader_program, vs);
    // Liga os shaders ao programa de shader, tornando-o pronto para a execução.
    glLinkProgram(shader_program);

    // 5.5 - Loop principal
    // define o programa de shader a ser usado para renderização.
    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        // Limpa o buffer de cor e o buffer de profundidade.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Vincula o VAO.
        glBindVertexArray(vao);
        // Desenha o triângulo.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // processa eventos de janela como entrada de teclado / mouse.
        glfwPollEvents();
        // Troca os buffers frontal e traseiro.
        glfwSwapBuffers(window);
    }

    // 6 - Ao final, terminar / remover GLFW da memória.
    glfwTerminate();
    return 0;
}
