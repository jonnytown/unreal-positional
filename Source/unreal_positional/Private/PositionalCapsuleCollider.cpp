// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalCapsuleCollider.h"
#include "collision/collider/CapsuleCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalCapsuleCollider::UPositionalCapsuleCollider()
{
	// ...
	Radius = 50;
	Length = 100;
}

Ref<Collider> UPositionalCapsuleCollider::CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform &transform)
{
	const auto pos = transform.TransformPosition(Center);
	const auto rot = transform.GetRotation() * Rotation.Quaternion();
	return world->createCollider<CapsuleCollider>(body, ToVec3(pos), ToQuat(rot), Density, StaticFriction, DynamicFriction, Bounciness, Radius, Length);
}

void UPositionalCapsuleCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		auto& collider = m_Collider.get();
		collider.pose.position = ToVec3(transform.TransformPosition(Center));
		collider.pose.rotation = ToQuat(transform.GetRotation() * Rotation.Quaternion());
	}
}

