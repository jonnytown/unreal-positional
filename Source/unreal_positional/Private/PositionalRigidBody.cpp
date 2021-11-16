// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalRigidBody.h"
#include "PositionalUtil.h"

// Sets default values
APositionalRigidBody::APositionalRigidBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APositionalRigidBody::CreateBody(TSoftObjectPtr<APositionalWorld>& world)
{
	if (!m_Body.valid() && world.IsValid())
	{
		auto transform = GetActorTransform();
		m_Body = world.Get()->GetPtr()->createBody<RigidBody>(ToVec3(transform.GetTranslation()), ToQuat(transform.GetRotation()));
		world.Get()->RegisterBody(this);
	}
}

void APositionalRigidBody::DestroyBody(TSoftObjectPtr<APositionalWorld>&world)
{
	if (m_Body.valid() && world.IsValid())
	{
		world.Get()->DeregisterBody(this);
		world.Get()->GetPtr()->destroyBody(m_Body);
		m_Body.reset();
	}
}

void APositionalRigidBody::PostLoad()
{
	Super::PostLoad();

	/*if (!World.IsValid())
	{
		World = APositionalWorld::Find(GetWorld());
	}*/

	CreateBody(World);
	
}

void APositionalRigidBody::PostActorCreated()
{
	Super::PostActorCreated();

	if (!World.IsValid())
	{
		World = APositionalWorld::Find(GetWorld());
	}

	CreateBody(World);
}

void APositionalRigidBody::Destroyed()
{
	DestroyBody(World);
	Super::Destroyed();
}

#if WITH_EDITOR
void APositionalRigidBody::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString name;
	PropertyChangedEvent.MemberProperty->GetName(name);

	if (name == TEXT("World"))
	{
		DestroyBody(m_PrevWorld);
		CreateBody(World);
		m_PrevWorld = World;
	}
}
#endif // WITH_EDITOR

void APositionalRigidBody::SyncTransform()
{
	if (m_Body.valid())
	{
		auto& body = m_Body.get();
		auto transform = GetActorTransform();
		transform.RemoveScaling();
		body.pose.position = ToVec3(transform.GetTranslation());
		body.pose.rotation = ToQuat(transform.GetRotation());
	}
}

void APositionalRigidBody::UpdateTransform()
{
	if (m_Body.valid())
	{
		const auto& body = m_Body.get();
		SetActorLocationAndRotation(ToFVector(body.pose.position), ToFQuaternion(body.pose.rotation));
	}
}

void APositionalRigidBody::UpdateMass()
{
	if (m_Body.valid())
	{
		auto& body = m_Body.get();
		body.updateMass();
		Mass = body.invMass ? 1.0/body.invMass : 0;
	}
}
