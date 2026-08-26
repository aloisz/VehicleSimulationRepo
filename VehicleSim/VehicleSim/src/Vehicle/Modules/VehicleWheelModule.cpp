#include "VehicleWheelModule.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"
#include "../../Render/DebugDraw.h"

#include <cmath>
#include <algorithm>

using namespace MathUtils;

namespace Vehicle 
{
	bool VehicleWheelModule::Initialize(VehicleContext& context)
	{
		if (!context.IsValid())
		{
			Log::Error("VehicleWheelModule: invalid context", Log::Category::Vehicle);
			return false;
		}

		if (context.Config->Wheels.empty())
		{
			Log::Error("VehicleWheelModule: config has no wheels", Log::Category::Vehicle);
			return false;
		}

		if (context.Runtime->Wheels.size() != context.Config->Wheels.size())
		{
			context.Runtime->Resize(context.Config->Wheels.size());
		}

		_initialized = true;
		return true;
	}

	void VehicleWheelModule::Update(float dt, VehicleContext& context)
	{
		if (!_initialized) return;

		for (size_t i = 0; i < context.Config->Wheels.size(); ++i) 
		{
			UpdateGroundDetection(context, i);
			UpdateSuspension(context, i);
			UpdateSlip(context, i);
			UpdateTireForces(context, i);
			IntegrateWheelSpin(dt, context, i);
		}
	}

	void VehicleWheelModule::Dispose()
	{
		_initialized = false;
	}

	void VehicleWheelModule::UpdateGroundDetection(VehicleContext& context, size_t index)
	{
		const WheelConfig& wc = context.Config->Wheels[index];
		WheelRuntime& wr = context.Runtime->Wheels[index];

		const btTransform& chassis = context.GetTransform();
		const btVector3 up = context.GetUp();

		// The anchor is where the suspension meets the chassis
		wr.AnchorWorld = chassis * context.DesignToBody(wc.LocalOffset);

		// The ray has to reach past the wheel own radius so that the wheel
		// surface is what touches the ground
		const float rayLength = wc.Suspension.Length + wc.Radius;
		const btVector3 rayFrom = wr.AnchorWorld;
		const btVector3 rayTo = rayFrom - up * rayLength;

		btCollisionWorld::ClosestRayResultCallback callback(rayFrom, rayTo);
		callback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
		callback.m_collisionFilterMask =
			btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter;

		context.World->rayTest(rayFrom, rayTo, callback);
		DebugDraw::AddRay(rayFrom, rayTo, callback.hasHit());

		const bool hit = callback.hasHit() &&
			callback.m_hitNormalWorld.dot(up) > MIN_GROUND_DOT;

		if (hit)
		{
			wr.Grounded = true;
			wr.HitPoint = callback.m_hitPointWorld;
			wr.HitNormal = callback.m_hitNormalWorld.normalized();

			const float hitDistance = (callback.m_hitPointWorld - rayFrom).length();

			// Distance from the anchor down to the wheel centre
			wr.CurrentLength = Clamp(hitDistance - wc.Radius, 0.0f, wc.Suspension.Length);
		}
		else
		{
			wr.Grounded = false;
			wr.HitNormal = up;
			wr.CurrentLength = wc.Suspension.Length; // Fully drooped
			wr.HitPoint = rayFrom - up * (wc.Suspension.Length + wc.Radius);
		}

		wr.Compression = wc.Suspension.Length - wr.CurrentLength;
		wr.WheelCenter = rayFrom - up * wr.CurrentLength;

		/*Log::Info("Length: '" + std::to_string(wr.CurrentLength) + "' ", Log::Category::Vehicle);
		Log::Info("Compression: '" + std::to_string(wr.Compression) + "' ", Log::Category::Vehicle);
		Log::Info("WheelCenter: '" 
			+ std::to_string(wr.WheelCenter.getX()) + "," 
			+ std::to_string(wr.WheelCenter.getY()) + "," 
			+ std::to_string(wr.WheelCenter.getZ()) + "' \n", Log::Category::Vehicle);*/
	}

	void VehicleWheelModule::UpdateSuspension(VehicleContext& context, size_t index)
	{
		const WheelConfig& wc = context.Config->Wheels[index];
		const SuspensionConfig& sc = wc.Suspension;
		WheelRuntime& wr = context.Runtime->Wheels[index];

		if (!wr.Grounded)
		{
			wr.WheelLoad = 0.0f;
			wr.SuspensionForce = btVector3(0.0f, 0.0f, 0.0f);
			return;
		}

		const btVector3 up = context.GetUp();
		const btVector3 relPos = wr.AnchorWorld - context.GetCenterOfMass();

		const btVector3 velocityAtAnchor = context.Body->getVelocityInLocalPoint(relPos);
		const float suspensionVelocity = velocityAtAnchor.dot(up);

		const float springForce = sc.SpringStrength * wr.Compression;
		const float damperForce = sc.DamperStrength * suspensionVelocity;

		float force = springForce - damperForce;

		// The suspension can push but never pull.
		force = Clamp(force, 0.0f, sc.MaxForce);

		wr.WheelLoad = force;
		wr.SuspensionForce = up * force;
	}

	void VehicleWheelModule::UpdateSlip(VehicleContext& context, size_t index)
	{
		
	}

	void VehicleWheelModule::UpdateTireForces(VehicleContext& context, size_t index)
	{
		
	}

	void VehicleWheelModule::IntegrateWheelSpin(float dt, VehicleContext& context, size_t index)
	{

	}
}