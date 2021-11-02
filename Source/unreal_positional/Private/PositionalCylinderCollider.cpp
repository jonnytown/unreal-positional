// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalCylinderCollider.h"
#include "collision/collider/CylinderCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalCylinderCollider::UPositionalCylinderCollider()
{
	// ...
	Radius = 50;
	Length = 100;
}

Ref<Collider> UPositionalCylinderCollider::CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform &transform)
{
	const auto pos = transform.GetTranslation() + Center;
	const auto rot = FQuat(Rotation) * transform.GetRotation();
	return world->createCollider<CylinderCollider>(body, ToVec3(pos), ToQuat(rot), Density, StaticFriction, DynamicFriction, Bounciness, Radius, Length);
}

void UPositionalCylinderCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		auto& collider = m_Collider.get();
		collider.pose.position = ToVec3(transform.GetTranslation() + Center);
		collider.pose.rotation = ToQuat(transform.GetRotation() * Rotation.Quaternion());
	}
}

