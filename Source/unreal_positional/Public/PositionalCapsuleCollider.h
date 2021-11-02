// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionalCollider.h"
#include "PositionalCapsuleCollider.generated.h"


UCLASS( ClassGroup=(Positional), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalCapsuleCollider : public UPositionalCollider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider|Pose")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider|Shape")
	double Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider|Shape")
	double Length;

	// Sets default values for this component's properties
	UPositionalCapsuleCollider();

protected:
	virtual Ref<Collider> CreateCollider(Positional::World* world, const Ref<Body> &body, const FTransform& transform) override;
	virtual void SyncTransform(const FTransform& transform) override;
};
