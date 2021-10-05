// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "simulation/World.h"
#include "EngineUtils.h"
#include "PositionalWorld.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogPositional, Log, All);

using namespace Positional;

class UPositionalCollider;
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

UCLASS(ClassGroup = (Custom))
class UNREAL_POSITIONAL_API APositionalWorld : public AActor
{
	friend class UPositionalCollider;
	friend class UPositionalBoxCollider;
	friend class UPositionalCapsuleCollider;
	friend class UPositionalSphereCollider;
	friend class APositionalRigidBody;

	GENERATED_BODY()

private:
	World* m_World;
	TMap<uint64, UPositionalCollider*> m_Colliders;
	TMap<uint64, APositionalRigidBody*> m_RigidBodies;
	FTimerHandle m_SimulationTimer;
	bool m_Simulating;
public:	

	UPROPERTY(EditAnywhere)
		float DeltaTime;

	UPROPERTY(EditAnywhere)
		uint8 SubSteps;

	UPROPERTY(EditAnywhere)
		FVector Gravity;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
		bool ShowDebugBoundsTree;

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
	Ref<Body> CreateRigidBody(APositionalRigidBody* body, const FVector& pos, const FQuat& rot);
	void DestroyRigidBody(const Ref<Body>& ptr);

	Ref<Collider> CreateSphereCollider(APositionalRigidBody *body, UPositionalCollider* component, const FVector &pos, const float &radius, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness);
	Ref<Collider> CreateCapsuleCollider(APositionalRigidBody* body, UPositionalCollider* component, const FVector& pos, const FQuat& rot, const float& radius, const float& length, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness);
	Ref<Collider> CreateBoxCollider(APositionalRigidBody* body, UPositionalCollider* component, const FVector& pos, const FQuat& rot, const FVector& extents, const float& density, const float& staticFriction, const float& dynamicFriction, const float& bounciness);
	void DestroyCollider(const Ref<Collider>& ptr);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void FireSimulation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
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


