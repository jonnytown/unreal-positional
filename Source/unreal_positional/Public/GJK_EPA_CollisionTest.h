// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PositionalWorld.h"
#include "GJK_EPA_CollisionTest.generated.h"

UCLASS()
class UNREAL_POSITIONAL_API AGJK_EPA_CollisionTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGJK_EPA_CollisionTest();

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr  <AActor> A;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr  <AActor> B;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr  <AActor> C;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr  <AActor> D;

	UPROPERTY(EditAnywhere)
		uint8 Count;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

};
