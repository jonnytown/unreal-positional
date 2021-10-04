// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionalCollider.h"
#include "PositionalCapsuleCollider.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalCapsuleCollider : public UPositionalCollider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FRotator Rotation;

	UPROPERTY(EditAnywhere)
		float Radius;

	UPROPERTY(EditAnywhere)
		float Length;

	// Sets default values for this component's properties
	UPositionalCapsuleCollider();

protected:
	virtual Store<Collider>::Ref CreateCollider(APositionalWorld* world, APositionalRigidBody* body, const FTransform& transform) override;
	virtual void SyncTransform(const FTransform& transform) override;
};
