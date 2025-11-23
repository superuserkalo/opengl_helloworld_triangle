#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <chrono>

float vertices[] = {
     0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
};

const char* vertexSource = 
R"glsl(
    #version 150 core

    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";


const char* fragmentSource = 
R"glsl(
    #version 150 core

    in vec3 Color;

    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

int main(int argc, char* argv[]) {
    // --- 1. SDL Setup ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow("OpenGL Triangle", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "GLAD failed" << std::endl;
        return -1;
    }

    // --- 2. VAO & VBO Setup (CRITICAL: ORDER MATTERS) ---

    // Step A: Generate and BIND the VAO first.
    // The VAO records the subsequent VBO bindings and VertexAttribPointer calls.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Step B: Generate and BIND the VBO.
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // <--- FIX: You missed this line!
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // --- 3. Shader Setup ---
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    // (Add compile check here in production)

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders (they are linked into the program now, we don't need the objects)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --- 4. Attributes & Uniforms ---

    // Now we tell OpenGL how to read the VBO we bound in Step B.
    // Because 'vao' is still bound, this setting is saved into the VAO.
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colAttrib);
    
    //The fifth parameter is set to 5*sizeof(float) now, because each vertex consists of 5 floating point attribute values. 
    // The offset of 2*sizeof(float) for the color attribute is there 
    // because each vertex starts with 2 floating point values for the position that it has to skip over.
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // Uniforms must be set AFTER linking and using the program
    glUseProgram(shaderProgram);

    std::cout << "Setup Complete. Error Check: " << glGetError() << std::endl;

    // --- 5. Main Loop ---
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Use the shader
        glUseProgram(shaderProgram);
        // 2. Bind the VAO (contains the VBO + Pointer config)
        glBindVertexArray(vao);
        // 3. Draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}