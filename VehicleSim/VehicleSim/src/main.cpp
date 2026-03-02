#include <iostream>
#include <GL/freeglut.h>
#include "Application.h"
#include "Ground.h"
#include "Vehicle.h"

Application& app = Application::getInstance();
Vehicle* vehicle = nullptr;

void setupCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    btVector3 vehiclePos = vehicle->GetVehiclePosition();
    btVector3 cameraOffset(15.0f, 15.0f, 15.0f); 

    btVector3 cameraPos = vehiclePos + cameraOffset;
    
    gluLookAt(
        cameraPos.getX(), cameraPos.getY(), cameraPos.getZ(),
        vehicle->GetVehiclePosition().getX(), vehicle->GetVehiclePosition().getY(), vehicle->GetVehiclePosition().getZ(),
        0, 1, 0
    );
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.update(1.0f / 60.0f);
    setupCamera();
    app.render();

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = { 10.0f, 20.0f, 10.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void handleKeypress(unsigned char key, int x, int y)
{
    if (key == 27) { // Escape key
        app.cleanup();
        exit(0);
    }
    
    if (vehicle)
    {
        float force = 50000.0f;

        switch (key)
        {
        case 'w': case 'W':
            vehicle->applyForce(btVector3(0, 0, -force));
            break;
        case 's': case 'S':
            vehicle->applyForce(btVector3(0, 0, force));
            break;
        case 'a': case 'A':
            vehicle->applyTorque(btVector3(0, 500, 0));
            break;
        case 'd': case 'D':
            vehicle->applyTorque(btVector3(0, -500, 0));
            break;
        case ' ':
            vehicle->applyForce(btVector3(0, 8000, 0));
            break;
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Vehicle Simulation");

    initGL();

    app.init();

    // Create game objects
    Ground* ground = new Ground(app.getPhysics().getWorld());
    app.addGameObject(ground);

    vehicle = new Vehicle(app.getPhysics().getWorld(), btVector3(0, 5, 0));
    app.addGameObject(vehicle);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(handleKeypress);
    glutMainLoop();

    app.cleanup();
    return 0;
}