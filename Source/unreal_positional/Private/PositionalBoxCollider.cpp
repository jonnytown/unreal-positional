// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalBoxCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalBoxCollider::UPositionalBoxCollider()
{
	// ...
	Extents = FVector(50, 50, 50);
}


Ref<Collider> UPositionalBoxCollider::CreateCollider(APositionalWorld* world, APositionalRigidBody* body, const FTransform& transform)
{
	const auto pos = transform.GetTranslation() + Center;
	const auto rot = FQuat(Rotation) * transform.GetRotation();
	return world->CreateBoxCollider(body, this, pos, rot, Extents, Density, StaticFriction, DynamicFriction, Bounciness);
}

void UPositionalBoxCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		auto& collider = m_Collider.get();
		collider.pose.position = ToVec3(transform.GetTranslation() + Center);
		collider.pose.rotation = ToQuat(transform.GetRotation() * Rotation.Quaternion());
	}
}

