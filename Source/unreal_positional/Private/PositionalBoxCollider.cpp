// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalBoxCollider.h"
#include "collision/collider/BoxCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalBoxCollider::UPositionalBoxCollider()
{
	// ...
	Extents = FVector(50, 50, 50);
}


Ref<Collider> UPositionalBoxCollider::CreateCollider(Positional::World* world, const Ref<Body> &body, const FTransform& transform)
{
	const auto pos = transform.TransformPosition(Center);
	const auto rot = transform.GetRotation() * Rotation.Quaternion();
	return world->createCollider<BoxCollider>(body, ToVec3(pos), ToQuat(rot), Density, StaticFriction, DynamicFriction, Bounciness, ToVec3(Extents));
}

void UPositionalBoxCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		auto& collider = m_Collider.get();
		collider.pose.position = ToVec3(transform.TransformPosition(Center));
		collider.pose.rotation = ToQuat(transform.GetRotation() * Rotation.Quaternion());
	}
}

