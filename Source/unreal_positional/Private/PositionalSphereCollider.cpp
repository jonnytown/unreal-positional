// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalSphereCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalSphereCollider::UPositionalSphereCollider()
{
	// ...
	Radius = 50.0f;
}

Ref<Collider> UPositionalSphereCollider::CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform &transform)
{
	const auto pos = transform.GetTranslation() + Center;
	return world->createSphereCollider(body, ToVec3(pos), Radius, Density, StaticFriction, DynamicFriction, Bounciness);
}

void UPositionalSphereCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		m_Collider.get().pose.position = ToVec3(transform.GetTranslation() + Center);
	}
}

