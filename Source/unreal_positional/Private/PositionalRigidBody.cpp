// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalRigidBody.h"
#include "PositionalUtil.h"

// Sets default values
APositionalRigidBody::APositionalRigidBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APositionalRigidBody::CreateBody(TSoftObjectPtr<APositionalWorld>& world)
{
	if (!m_Body.valid() && world.IsValid())
	{
		auto transform = GetActorTransform();
		m_Body = world.Get()->CreateRigidBody(this, transform.GetTranslation(), transform.GetRotation());
	}
}


void APositionalRigidBody::DestroyBody(TSoftObjectPtr<APositionalWorld>&world)
{
	if (m_Body.valid() && world.IsValid())
	{
		world.Get()->DestroyRigidBody(m_Body);
		m_Body.reset();
	}
}

void APositionalRigidBody::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();

	if (!World.IsValid())
	{
		World = APositionalWorld::Find(GetWorld());
	}

	CreateBody(World);
}

void APositionalRigidBody::PostUnregisterAllComponents()
{
	Super::PostUnregisterAllComponents();
	DestroyBody(World);
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
		body.pose.position = ToVec3(transform.GetTranslation());
		body.pose.rotation = ToQuat(transform.GetRotation());
	}
}

void APositionalRigidBody::UpdateTransform()
{
	if (m_Body.valid())
	{
		auto& body = m_Body.get();
		SetActorLocationAndRotation(ToFVector(body.pose.position), ToFQuaternion(body.pose.rotation));
	}
}

