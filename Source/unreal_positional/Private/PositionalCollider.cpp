// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalCollider.h"
#include "PositionalUtil.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPositionalCollider::UPositionalCollider()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Density = 0.0012;
	StaticFriction = 0.2;
	DynamicFriction = 0.2;
	Bounciness = 0.2;
}

void UPositionalCollider::OnRegister()
{
	Super::OnRegister();

	if (!World.IsValid())
	{
		World = APositionalWorld::Find(GetWorld());
	}

	CreateCollider(World);
}

void UPositionalCollider::OnUnregister()
{
	Super::OnUnregister();

	DestroyCollider(World);
}

void UPositionalCollider::CreateCollider(const TSoftObjectPtr<APositionalWorld> &world)
{
	if (!m_Collider.valid() && world.IsValid())
	{
		// TODO: find body in ancestor
		auto body = Cast<APositionalRigidBody>(GetOwner());
		m_Collider = CreateCollider(world.Get()->GetPtr(), body ? body->GetRef() : Body::null, GetRelativeTransform());
		world.Get()->RegisterCollider(this);
	}
}

void UPositionalCollider::DestroyCollider(const TSoftObjectPtr<APositionalWorld> &world)
{
	if (m_Collider.valid() && world.IsValid())
	{
		world.Get()->DeregisterCollider(this);
		world.Get()->GetPtr()->destroyCollider(m_Collider);
		m_Collider.reset();
	}
}

#if WITH_EDITOR
void UPositionalCollider::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString name;
	PropertyChangedEvent.MemberProperty->GetName(name);

	if (name == TEXT("World"))
	{
		DestroyCollider(m_PrevWorld);
		CreateCollider(World);
		m_PrevWorld = World;
		return;
	}

	if (m_Collider.valid() && name == TEXT("Density") && m_Collider.get().body().valid())
	{
		m_Collider.get().density = Density;
		m_Collider.get().body().get().updateMass();
		return;
	}

	if (m_Collider.valid() && name == TEXT("StaticFriction") && m_Collider.get().body().valid())
	{
		m_Collider.get().staticFriction = StaticFriction;
		return;
	}

	if (m_Collider.valid() && name == TEXT("DynamicFriction") && m_Collider.get().body().valid())
	{
		m_Collider.get().dynamicFriction = DynamicFriction;
		return;
	}

	if (m_Collider.valid() && name == TEXT("Bounciness") && m_Collider.get().body().valid())
	{
		m_Collider.get().restitution = Bounciness;
		return;
	}
}
#endif // WITH_EDITOR

FTransform UPositionalCollider::GetRelativeTransform() const
{
	// TODO: use relative transform to body if body exists
	auto body = Cast<APositionalRigidBody>(GetOwner());
	if (body)
	{
		return FTransform::Identity;
	}
	return GetOwner()->GetActorTransform();
}

void UPositionalCollider::SyncTransform()
{
	if (m_Collider.valid())
	{
		SyncTransform(GetRelativeTransform());
	}
}

bool UPositionalCollider::Raycast(const FVector& rayOrigin, const FVector& rayNormal, const float& maxDistance, FVector& outPoint, FVector& outNormal, float& outDistance)
{
	if (m_Collider.valid())
	{
		SyncTransform();

		Vec3 p, n;
		Float d;
		if (m_Collider.get().raycast(Ray(ToVec3(rayOrigin), ToVec3(rayNormal)), maxDistance, p, n, d))
		{
			outPoint = ToFVector(p);
			outNormal = ToFVector(n);
			outDistance = d;
			return true;
		}
	}
	return false;
}

void UPositionalCollider::GetBounds(FVector& outCenter, FVector& outExtents)
{
	if (m_Collider.valid())
	{
		SyncTransform();

		const Bounds& bounds = m_Collider.get().bounds();
		outCenter = ToFVector(bounds.center);
		outExtents = ToFVector(bounds.extents());
	}
}

