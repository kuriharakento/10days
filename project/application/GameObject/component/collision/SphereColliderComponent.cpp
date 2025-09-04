#include "SphereColliderComponent.h"
#include "application/GameObject/base/GameObject.h"

SphereColliderComponent::SphereColliderComponent(GameObject* owner)
    : ICollisionComponent(owner), sphere_(), collisionPosition_(0, 0, 0)
{
}

const Sphere& SphereColliderComponent::GetSphere() const
{
    return sphere_;
}

void SphereColliderComponent::SetSphere(const Sphere& s)
{
    sphere_ = s;
}

void SphereColliderComponent::Update(GameObject* owner)
{
	if (owner)
	{
		sphere_.center = owner->GetPosition();
	}
}

ColliderType SphereColliderComponent::GetColliderType() const
{
    return ColliderType::Sphere;
}
