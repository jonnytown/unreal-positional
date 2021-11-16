// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalSphereCollider.h"
#include "collision/collider/SphereCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalSphereCollider::UPositionalSphereCollider()
{
	// ...
	Radius = 50.0f;
}

Ref<Collider> UPositionalSphereCollider::CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform &transform)
{
	const auto pos = transform.TransformPosition(Center);
	return world->createCollider<SphereCollider>(body, ToVec3(pos), Quat::identity, Density, StaticFriction, DynamicFriction, Bounciness, Radius);
}

void UPositionalSphereCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		m_Collider.get().pose.position = ToVec3(transform.TransformPosition(Center));
	}
}

