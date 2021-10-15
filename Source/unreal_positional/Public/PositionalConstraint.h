// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PositionalWorld.h"
#include "PositionalRigidBody.h"
#include "PositionalConstraint.generated.h"


UCLASS( ClassGroup=(Custom), Abstract )
class UNREAL_POSITIONAL_API UPositionalConstraint : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<APositionalWorld> World;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<APositionalRigidBody> ConnectedBody;

	// Sets default values for this component's properties
	UPositionalConstraint();

protected:
	Ref<Constraint> m_Constraint;
	TSoftObjectPtr<APositionalWorld> m_PrevWorld;
	
	virtual Ref<Constraint> CreateConstraint(APositionalWorld *world, APositionalRigidBody *bodyA, APositionalRigidBody *bodyB)
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
