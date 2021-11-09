// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unreal_positional.h"
#include "Components/ActorComponent.h"
#include "PositionalWorld.h"
#include "PositionalRigidBody.h"
#include "PositionalCollider.generated.h"

UCLASS(ClassGroup = (Positional), Abstract)
class UNREAL_POSITIONAL_API UPositionalCollider : public UActorComponent
{
	friend class APositionalWorld;

	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, Category="Collider")
	TSoftObjectPtr<APositionalWorld> World;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider|Material")
	double Density;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider|Material")
	double StaticFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider|Material")
	double DynamicFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider|Material")
	double Bounciness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider|Pose")
	FVector Center;

	// Sets default values for this component's properties
	UPositionalCollider();

protected:
	Ref<Collider> m_Collider;
	TSoftObjectPtr<APositionalWorld> m_PrevWorld;

	Ref<Collider> GetRef() { return m_Collider; }

	FTransform GetRelativeTransform() const;
	virtual void SyncTransform(const FTransform& transform)
	{
		UE_LOG(LogPositional, Fatal, TEXT("SyncTransform not implemented!"));
	}

	virtual Ref<Collider> CreateCollider(Positional::World *world, const Ref<Body> &body, const FTransform& transform)
	{ 
		UE_LOG(LogPositional, Fatal, TEXT("CreateCollider not implemented!"));
		return Ref<Collider>();
	}

	void CreateCollider(const TSoftObjectPtr<APositionalWorld> &world);
	void DestroyCollider(const TSoftObjectPtr<APositionalWorld> &world);

public:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR

	bool Raycast(const FVector& rayOrigin, const FVector& rayNormal, const float& maxDistance, FVector& outPoint, FVector& outNormal, float& outDistance);
	void GetBounds(FVector& outCenter, FVector& outExtents);
	void SyncTransform();

};
