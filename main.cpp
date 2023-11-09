#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

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

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing glew!\n";
        std::cerr << glewGetErrorString(err) << '\n';
        return 1;
    }

    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutIdleFunc(render);

    glutMainLoop();

    return 0;
}