#pragma once

#include "../Core/Actor.h"


#include "Modules/VehicleInputModule.h"
#include "Modules/VehicleEngineModule.h"
#include "Modules/VehicleAerodynamicsModule.h"
#include "Modules/VehicleWheelModule.h"
#include "Modules/VehicleDrivetrainModule.h"
#include "Modules/VehiclePhysicsModule.h"
#include "Data/VehicleConfig.h"
#include "Data/VehicleRuntime.h"

namespace Vehicle {


	class VehicleCore : public Actor
	{
	public:
		VehicleCore(btDiscreteDynamicsWorld* world,
			const VehicleConfig& config,
			const btVector3& spawnPosition,
			float spawnYaw = 0.0f);

		~VehicleCore() override;

		// === Actor lifecycle ===
		void Awake() override;
		void Start() override;
		void OnDestroy() override;

		void Tick(float dt) override;
		void FixedTick(float fdt) override;
		void LateTick(float dt) override;

		void Render() override;

		btRigidBody* GetRigidBody() override { return _rigidBody; }
		const btRigidBody* GetRigidBody() const override { return _rigidBody; }

		// === Modules accessor ===
		VehicleInputModule& GetInputModule() { return _input; }
		VehicleEngineModule& GetEngineModule() { return _engine; }
		VehicleAerodynamicsModule& GetAerodynamicsModule() { return _aerodynamics; }
		VehicleWheelModule& GetWheelModule() { return _wheel; }
		VehicleDrivetrainModule& GetDrivetrainModule() { return _drivetrain; }
		VehiclePhysicsModule& GetPhysicsModule() { return _physics; }

		// === Data accessor ===
		const VehicleConfig& GetConfig() const { return _config; }
		const VehicleRuntime& GetRuntime() const { return _runtime; }

		bool IsOperational() const { return _operational; }

		btVector3 GetPosition() const;
		btVector3 GetForward() const;

		/// <summary>
		/// Reset the vehicle back at its spawn point
		/// </summary>
		void Reset();

	private:
		bool InitializeModules();
		bool CreateRigidBody();
		void DestroyRigidBody();

		void UpdateRuntimeMotion();

		void RenderChassis() const;
		void RenderWheels() const;

		// === Modules ===
		VehicleInputModule _input;
		VehicleEngineModule _engine;
		VehicleAerodynamicsModule _aerodynamics;
		VehicleWheelModule _wheel;
		VehicleDrivetrainModule _drivetrain;
		VehiclePhysicsModule _physics;

		// === Data ===
		VehicleConfig _config;
		VehicleContext _context;
		VehicleRuntime _runtime;

		// === Bullet resources ===
		btDiscreteDynamicsWorld* _world = nullptr;
		btRigidBody* _rigidBody = nullptr;
		btCompoundShape* _compoundShape = nullptr;
		btBoxShape* _boxShape = nullptr;
		btDefaultMotionState* _motionState = nullptr;

		btTransform _spawnTransform;
		bool _operational = false;
	};
}
