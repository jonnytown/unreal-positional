// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalSphereCollider.h"
#include "PositionalUtil.h"

// Sets default values for this component's properties
UPositionalSphereCollider::UPositionalSphereCollider()
{
	// ...
	Radius = 50.0f;
}

Store<Collider>::Ref UPositionalSphereCollider::CreateCollider(APositionalWorld* world, APositionalRigidBody* body, const FTransform& transform)
{
	const auto pos = transform.GetTranslation() + Center;
	return world->CreateSphereCollider(body, this, pos, Radius, Density, StaticFriction, DynamicFriction, Bounciness);
}

void UPositionalSphereCollider::SyncTransform(const FTransform& transform)
{
	if (m_Collider.valid())
	{
		m_Collider.get().pose.position = ToVec3(transform.GetTranslation() + Center);
	}
}
