#include <GL/freeglut.h>
#include "Physics.h"

Physics physics;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    physics.stepSimulation(1.0f / 60.0f);

    // Simple debug draw later (optional)

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Vehicle Simulation");

    initGL();
    physics.init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
