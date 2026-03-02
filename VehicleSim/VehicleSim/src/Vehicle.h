#pragma once

#include "GameObject.h"



/*struct WheelData 
{
public:
    
    FVector WheelOffset = FVector::ZeroVector;
    float WheelRadius = 35.0f;
    
    float SuspensionLength = 50.0f;

    // Physics State
    bool bIsGrounded = false;
    FVector HitPoint = FVector::ZeroVector;
    FVector HitNormal = FVector::UpVector;
    ECustomSurfaceType SurfaceType = ECustomSurfaceType::Road;
    float SuspensionCompression = 0.0f;
    
    float WheelAngularVelocity = 0.0f; // rad/s
    float WheelLoad = 0.0f; // Normal force from suspension
    float SlipRatio = 0.0f; // Longitudinal slip
    float SlipAngle = 0.0f; // Lateral slip angle in radians
    float CamberAngle = 0.0f;

    // Tire force
    FVector TireForce = FVector::ZeroVector;
    float LongitudinalForce = 0.0f;
    float LateralForce = 0.0f;

    FWheelData()
    {
        WheelOffset = FVector::ZeroVector;
        WheelRadius = 35.0f;
        SuspensionLength = 50.0f;
        bIsGrounded = false;
        SuspensionCompression = 0.0f;
        WheelAngularVelocity = 0.0f;
        WheelLoad = 0.0f;
        SlipRatio = 0.0f;
        SlipAngle = 0.0f;
        CamberAngle = 0.0f;
        TireForce = FVector::ZeroVector;
        LongitudinalForce = 0.0f;
        LateralForce = 0.0f;
    }
};*/



class Vehicle : public GameObject
{

public:
    Vehicle(btDiscreteDynamicsWorld* world, const btVector3& position);
    ~Vehicle();
    
    void render() override;
    void update(float deltaTime) override;

    void applyForce(const btVector3& force);
    void applyTorque(const btVector3& torque);

    
    btVector3 GetVehiclePosition();

private:
    void renderBox(float width, float height, float depth);
    

    glm::vec3 bodySize;
    glm::vec3 wheelSize;
    btDiscreteDynamicsWorld* worldRef;


    btScalar vehicleMass;
    std::vector<float> previousSuspensionCompressions = { 0,0,0,0 };
};

