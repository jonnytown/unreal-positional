// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PositionalCollider.h"
#include "PositionalSphereCollider.generated.h"

using namespace Positional;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalSphereCollider : public UPositionalCollider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Radius;

	// Sets default values for this component's properties
	UPositionalSphereCollider();
protected:
	virtual Ref<Collider> CreateCollider(APositionalWorld* world, APositionalRigidBody* body, const FTransform& transform) override;
	virtual void SyncTransform(const FTransform &transform) override;
};
