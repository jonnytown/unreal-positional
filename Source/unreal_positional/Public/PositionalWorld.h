// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "simulation/World.h"
#include "EngineUtils.h"
#include "PositionalWorld.generated.h"

using namespace Positional;

class UPositionalCollider;
class UPositionalConstraint;
class APositionalRigidBody;

struct PositionalRaycastResult
{
	UPositionalCollider* collider;
	FVector point;
	FVector normal;
	float distance;

	PositionalRaycastResult(UPositionalCollider* _collider, const FVector& _point, const FVector& _normal, const float& _distance)
		: collider(_collider),
		point(_point),
		normal(_normal),
		distance(_distance) {}
};

UCLASS(ClassGroup = (Positional))
class UNREAL_POSITIONAL_API APositionalWorld : public AActor
{
	friend class APositionalRigidBody;
	friend class UPositionalCollider;
	friend class UPositionalConstraint;

	GENERATED_BODY()

private:
	World* m_World;
	TMap<uint64, APositionalRigidBody*> m_RigidBodies;
	TMap<uint64, UPositionalCollider*> m_Colliders;
	TMap<uint64, UPositionalConstraint*> m_Constraints;
	FTimerHandle m_SimulationTimer;
	bool m_Simulating;
public:	

	UPROPERTY(EditAnywhere)
		double DeltaTime;

	UPROPERTY(EditAnywhere)
		uint8 SubSteps;

	UPROPERTY(EditAnywhere)
		FVector Gravity;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
		bool ShowDebugBoundsTree;

	UPROPERTY(EditAnywhere)
		bool ShowDebugVelocities;

	UPROPERTY(EditAnywhere)
		bool ShowDebugCollisions;

	UPROPERTY(EditAnywhere)
		bool ShowDebugCollisionCSOs;

	UPROPERTY(EditAnywhere)
		bool ShowDebugMassProperties;

	UPROPERTY(EditAnywhere)
		bool DebugManuallyStepSimulation;

	UPROPERTY(EditAnywhere)
		bool DebugStep;
#endif // WITH_EDITORONLY_DATA

	APositionalWorld();
	~APositionalWorld();

	void Raycast(const FVector& rayOrigin, const FVector& rayNormal, const unsigned int& mask, const float& maxDistance, TArray<PositionalRaycastResult> &results) const;

protected:
	World *GetPtr() const { return m_World; }

	void RegisterBody(APositionalRigidBody *body);
	void DeregisterBody(APositionalRigidBody *body);

	void RegisterCollider(UPositionalCollider *collider);
	void DeregisterCollider(UPositionalCollider *collider);

	void RegisterConstraint(UPositionalConstraint *constraint);
	void DeregisterConstraint(UPositionalConstraint *constraint);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void FireSimulation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void BeginDestroy() override;
	void SyncTransforms();
	void Simulate();

	inline static APositionalWorld* Find(UWorld* world)
	{
		for (TActorIterator<APositionalWorld> it(world); it; ++it)
		{
			return *it;
		}
		return nullptr;
	}
};


