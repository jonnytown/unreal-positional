// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional.h"
#include "Components/ActorComponent.h"
#include "PositionalWorld.h"
#include "PositionalRigidBody.h"
#include "PositionalConstraint.generated.h"

UCLASS( ClassGroup=(Positional), Abstract )
class UNREAL_POSITIONAL_API UPositionalConstraint : public UActorComponent
{
	friend class APositionalWorld;

	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, Category = "Constraint")
	TSoftObjectPtr<APositionalWorld> World;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint")
	TSoftObjectPtr<APositionalRigidBody> ConnectedBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint")
	bool IgnoreCollisions;

	// Sets default values for this component's properties
	UPositionalConstraint();

protected:
	Ref<Constraint> m_Constraint;
	TSoftObjectPtr<APositionalWorld> m_PrevWorld;

	Ref<Constraint> GetRef() { return m_Constraint; }
	
	virtual Ref<Constraint> CreateConstraint(Positional::World *world, const Ref<Body> &bodyA, const Ref<Body> &bodyB)
	{
		UE_LOG(LogPositional, Fatal, TEXT("CreateConstraint not implemented!"));
		return Ref<Constraint>();
	}

	void CreateConstraint(const TSoftObjectPtr<APositionalWorld> &world);
	void DestroyConstraint(const TSoftObjectPtr<APositionalWorld> &world);

public:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
