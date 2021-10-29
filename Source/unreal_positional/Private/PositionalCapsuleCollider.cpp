// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalCapsuleCollider.h"
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
	const auto pos = transform.GetTranslation() + Center;
	const auto rot = FQuat(Rotation) * transform.GetRotation();
	return world->createCapsuleCollider(body, ToVec3(pos), ToQuat(rot), Radius, Length, Density, StaticFriction, DynamicFriction, Bounciness);
}

void UPositionalCapsuleCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		auto& collider = m_Collider.get();
		collider.pose.position = ToVec3(transform.GetTranslation() + Center);
		collider.pose.rotation = ToQuat(transform.GetRotation() * Rotation.Quaternion());
	}
}

