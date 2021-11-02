// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PositionalCollider.h"
#include "PositionalSphereCollider.generated.h"

using namespace Positional;

UCLASS( ClassGroup=(Positional), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalSphereCollider : public UPositionalCollider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Collider|Shape")
		double Radius;

	// Sets default values for this component's properties
	UPositionalSphereCollider();
protected:
	virtual Ref<Collider> CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform &transform) override;
	virtual void SyncTransform(const FTransform &transform) override;
};
