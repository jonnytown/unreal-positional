// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PositionalWorld.h"
#include "simulation/RigidBody.h"
#include "PositionalRigidBody.generated.h"

UCLASS()
class UNREAL_POSITIONAL_API APositionalRigidBody : public AActor
{
	friend class APositionalWorld;
	friend class UPositionalCollider;
	friend class UPositionalConstraint;

	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<APositionalWorld> World;

	// Sets default values for this actor's properties
	APositionalRigidBody();

protected:
	Ref<Body> m_Body;
	TSoftObjectPtr<APositionalWorld> m_PrevWorld;


	Ref<Body> GetRef() const { return m_Body; }
	void CreateBody(TSoftObjectPtr<APositionalWorld>& world);
	void DestroyBody(TSoftObjectPtr<APositionalWorld>& world);

	void UpdateTransform();
public:	
	virtual void PreRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	void SyncTransform();
};
