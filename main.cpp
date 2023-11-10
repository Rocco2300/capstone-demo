#include <iostream>

// clang-format off
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <fstream>
#include <sstream>
// clang-format on

const std::string Path = "C:/Users/grigo/Repos/capstone-demo/";

static std::string readShader(const std::string& filePath) {
    std::ifstream in(filePath);
    std::stringstream ss;

    if (!in) {
        std::cerr << "Could not open file " << filePath << '\n';
    }

    ss << in.rdbuf();
    in.close();

    return ss.str();
}

static unsigned int compileShader(unsigned int type,
                                  const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        auto shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        std::cerr << "Failed to compile " << shaderType << " shader\n";
        std::cerr << message << '\n';

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    float ratio = width * 1.f / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45.f, ratio, 0.1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 200);
    glutInitWindowSize(400, 400);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Capstone Demo");

    // clang-format off
    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3
    };
    // clang-format on

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing glew!\n";
        std::cerr << glewGetErrorString(err) << '\n';
        return 1;
    }


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*) 0);
    std::string vertexShader = readShader(Path + "shaders/triangle.vert");
    std::string fragmentShader = readShader(Path + "shaders/triangle.frag");

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float),
                 &indices[0], GL_STATIC_DRAW);

    unsigned int shader;
    shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutIdleFunc(render);

    glutMainLoop();

    return 0;
}