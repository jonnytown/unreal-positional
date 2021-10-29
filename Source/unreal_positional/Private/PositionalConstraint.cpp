// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalConstraint.h"

// Sets default values for this component's properties
UPositionalConstraint::UPositionalConstraint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPositionalConstraint::OnRegister()
{
	Super::OnRegister();

	if (!World.IsValid())
	{
		World = APositionalWorld::Find(GetWorld());
	}

	CreateConstraint(World);
}

void UPositionalConstraint::OnUnregister()
{
	Super::OnUnregister();
	DestroyConstraint(World);
}

void UPositionalConstraint::CreateConstraint(const TSoftObjectPtr<APositionalWorld> &world)
{
	if (!m_Constraint.valid() && world.IsValid())
	{
		auto body = Cast<APositionalRigidBody>(GetOwner());
		if (body)
		{
			m_Constraint = CreateConstraint(world.Get()->GetPtr(), body->GetRef(), ConnectedBody.IsValid() ? ConnectedBody.Get()->GetRef() : Body::null);
			world.Get()->RegisterConstraint(this);
		}
		else
		{
			UE_LOG(LogPositional, Warning, TEXT("Must add positional constraint to positional body actor"));
		}
	}
}

void UPositionalConstraint::DestroyConstraint(const TSoftObjectPtr<APositionalWorld> &world)
{
	if (m_Constraint.valid() && world.IsValid())
	{
		world.Get()->DeregisterConstraint(this);
		world.Get()->GetPtr()->destroyConstraint(m_Constraint);
		m_Constraint.reset();
	}
}

#if WITH_EDITOR
void UPositionalConstraint::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString name;
	PropertyChangedEvent.MemberProperty->GetName(name);

	if (name == TEXT("World"))
	{
		DestroyConstraint(m_PrevWorld);
		CreateConstraint(World);
		m_PrevWorld = World;
		return;
	}

	if (m_Constraint.valid() && name == TEXT("ConnectedBody"))
	{
		m_Constraint.get().bodyB = ConnectedBody.IsValid() ? ConnectedBody.Get()->GetRef() : Body::null;
		return;
	}
}
#endif // WITH_EDITOR