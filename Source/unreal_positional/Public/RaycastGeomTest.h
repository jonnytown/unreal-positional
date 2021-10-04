// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "PositionalWorld.h"
#include "RaycastGeomTest.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableActor) )
class UNREAL_POSITIONAL_API ARaycastGeomTest : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ARaycastGeomTest();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr  <AActor> RayOrigin;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr  <AActor> RayDirection;

	UPROPERTY(EditAnywhere)
	float Distance;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr  <APositionalWorld> World;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
};
