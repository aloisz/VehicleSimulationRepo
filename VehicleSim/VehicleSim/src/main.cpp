#include <GL/freeglut.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/MathUtils.h"
#include "Objects/Ground.h"
#include "Vehicle/VehicleCore.h"
#include "Vehicle/Data/VehicleConfigLoader.h"
#include "Render/DebugDraw.h"

using namespace MathUtils;

namespace
{
    Application& g_app = Application::GetInstance();
    Vehicle::VehicleCore* g_vehicle = nullptr;

    int g_windowWidth = 1280;
    int g_windowHeight = 720;

    int g_lastElapsedMs = 0;

    btVector3 g_cameraPosition(0.0f, 6.0f, -12.0f);
    btVector3 g_cameraTarget(0.0f, 0.0f, 0.0f);
    bool g_cameraInitialized = false;

    const char* DEFAULT_CONFIG_PATHS[] = {
        "config/vehicle.json",
        "../config/vehicle.json",
        "../../config/vehicle.json"
    };

    void SetupProjection()
    {
        const float aspect = g_windowHeight > 0
            ? static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight)
            : 1.0f;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(55.0, aspect, 0.1, 2000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(g_cameraPosition.getX(), g_cameraPosition.getY(), g_cameraPosition.getZ(),
            g_cameraTarget.getX(), g_cameraTarget.getY(), g_cameraTarget.getZ(),
            0.0, 1.0, 0.0);
    }

    void Display()
    {
        const int nowMs = glutGet(GLUT_ELAPSED_TIME);
        float dt = static_cast<float>(nowMs - g_lastElapsedMs) / 1000.0f;
        g_lastElapsedMs = nowMs;

        // First frame, or a paused window => do not hand a garbage dt to the sim
        if (dt <= 0.0f || dt > 0.25f) dt = 1.0f / 60.0f;

        g_app.Update(dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SetupProjection();

        g_app.Render();

        glutSwapBuffers();
    }

    void Idle()
    {
        glutPostRedisplay();
    }

    void Reshape(int width, int height)
    {
        g_windowWidth = width;
        g_windowHeight = height > 0 ? height : 1;
        glViewport(0, 0, g_windowWidth, g_windowHeight);
    }

    void InitGL()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);

        const GLfloat lightPos[] = { 0.4f, 1.0f, 0.3f, 0.0f };
        const GLfloat lightAmbient[] = { 0.35f, 0.35f, 0.38f, 1.0f };
        const GLfloat lightDiffuse[] = { 0.85f, 0.85f, 0.80f, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

        glClearColor(0.10f, 0.11f, 0.14f, 1.0f);
    }

    Vehicle::VehicleConfig LoadVehicleConfig(int argc, char** argv)
    {
        Vehicle::VehicleConfig config = Vehicle::VehicleConfig::MakeDefault();

        std::string explicitPath;
        if (argc > 1 && argv[1] && argv[1][0] != '-') 
        {
            explicitPath = argv[1];
        }

        std::string error;

        if (!explicitPath.empty())
        {
            if (Vehicle::VehicleConfigLoader::LoadFromFile(explicitPath, config, error))
                return config;

            Log::Error(error, Log::Category::Config);
            Log::Warning("Falling back to the built in default config", Log::Category::Config);
            return Vehicle::VehicleConfig::MakeDefault();
        }

        for (const char* path : DEFAULT_CONFIG_PATHS)
        {
            Vehicle::VehicleConfig candidate = Vehicle::VehicleConfig::MakeDefault();
            if (Vehicle::VehicleConfigLoader::LoadFromFile(path, candidate, error))
                return candidate;
        }

        Log::Warning("No vehicle.json found on any default path, using the built in config",
            Log::Category::Config);
        return config;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(g_windowWidth, g_windowHeight);
    glutCreateWindow("Vehicle Simulation");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    InitGL();

    g_app.Init(1.0f / 120.0f);

    Ground* ground = new Ground(g_app.GetPhysics().GetWorld());
    g_app.AddActor(ground);

    const Vehicle::VehicleConfig config = LoadVehicleConfig(argc, argv);

    g_vehicle = new Vehicle::VehicleCore(
        g_app.GetPhysics().GetWorld(), config, btVector3(0.0f, 1.5f, 0.0f), 0.0f);
    g_app.AddActor(g_vehicle);

    if (!g_vehicle->IsOperational()) 
    {
        Log::Error("Vehicle failed to initialize", Log::Category::Vehicle);
    }
        

    glutIgnoreKeyRepeat(1);

    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);

    g_lastElapsedMs = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();

    g_app.Cleanup();
    g_vehicle = nullptr;

    return 0;
}
