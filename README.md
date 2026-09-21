# Native C++ Vehicle Physics Simulation

**Work in Progress**

A native C++ vehicle physics simulation focused on building a modular, deterministic, and configurable vehicle simulation architecture from scratch.

The project explores the implementation of a complete vehicle physics pipeline, from engine and drivetrain simulation to suspension, tire friction, and aerodynamics.

## Technologies

* **C++17**
* **Bullet3** — rigid-body physics and collision
* **FreeGLUT / OpenGL** — visualization and simulation interface
* **Dear ImGui** — debugging and simulation controls
* **nlohmann/json** — runtime configuration
* **CMake** — build system

## Features

* Runtime-configurable vehicle parameters through JSON
* Modular simulation architecture using a shared `VehicleContext`
* Engine, drivetrain, wheel, suspension, tire, and aerodynamics systems
* Fixed **120 Hz** physics simulation loop
* Deterministic physics stepping
* Separated control and force-application phases
* Vehicle test bench for validating individual physics behaviors

## Architecture

The simulation is organized around a shared `VehicleContext`, allowing individual modules to access the vehicle's configuration, runtime state, and physics representation without tightly coupling the systems together.

```text
VehicleCore
├── VehicleInputModule
├── VehicleEngineModule
├── VehicleDrivetrainModule
├── VehicleWheelModule
├── VehiclePhysicsModule
└── VehicleAerodynamicsModule
```
