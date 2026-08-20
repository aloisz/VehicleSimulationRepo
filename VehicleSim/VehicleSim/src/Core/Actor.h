#pragma once

#include <btBulletDynamicsCommon.h>
#include <string>

class Actor
{
public:
    Actor() = default;
    virtual ~Actor() = default;

    Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

    /// <summary>
    /// Called at actor initialization
    /// </summary>
    virtual void Awake() {}
    virtual void Start() {}
    virtual void OnDestroy() {}

    virtual void Tick(float /*dt*/) {}
    virtual void FixedTick(float /*fdt*/) {}
    virtual void LateTick(float /*dt*/) {}

    virtual void Render() = 0;

    void GetGLTransform(btScalar* out) const
    {
        if (const btRigidBody* body = GetRigidBody())
        {
            body->getCenterOfMassTransform().getOpenGLMatrix(out);
            return;
        }

        btTransform identity;
        identity.setIdentity();
        identity.getOpenGLMatrix(out);
    }

    virtual btRigidBody* GetRigidBody() { return nullptr; }
    virtual const btRigidBody* GetRigidBody() const { return nullptr; }

    const std::string& GetName() const { return _name; }
    void SetName(const std::string& name) { _name = name; }

    bool IsStarted() const { return _started; }
    void MarkStarted() { _started = true; }

private:
    std::string _name = "Actor";
    bool _started = false;
};
