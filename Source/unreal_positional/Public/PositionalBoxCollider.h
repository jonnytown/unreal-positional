// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionalCollider.h"
#include "PositionalBoxCollider.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalBoxCollider : public UPositionalCollider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FRotator Rotation;

	UPROPERTY(EditAnywhere)
		FVector Extents;

	// Sets default values for this component's properties
	UPositionalBoxCollider();

protected:
	virtual Ref<Collider> CreateCollider(APositionalWorld* world, APositionalRigidBody* body, const FTransform& transform) override;
	virtual void SyncTransform(const FTransform& transform) override;
};
