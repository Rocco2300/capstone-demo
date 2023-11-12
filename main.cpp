#include <iostream>

// clang-format off
#include "Mesh.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std::chrono;
// clang-format on

const std::string Path = "C:/Users/grigo/Repos/capstone-demo/";

Mesh mesh;

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
    auto start = high_resolution_clock::now();
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << static_cast<int>(1 / (duration.count() / 1'000'000.f)) << '\n';
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

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing glew!\n";
        std::cerr << glewGetErrorString(err) << '\n';
        return 1;
    }

    mesh.generate(1024, 1024);

    mesh.bind();

    std::string vertexShader = readShader(Path + "shaders/mesh.vert");
    std::string fragmentShader = readShader(Path + "shaders/mesh.frag");

    unsigned int shader;
    shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutIdleFunc(render);

    glutMainLoop();

    return 0;
}