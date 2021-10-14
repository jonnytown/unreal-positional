// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PositionalWorld.h"
#include "PositionalRigidBody.h"
#include "PositionalCollider.generated.h"



UCLASS( ClassGroup=(Custom), Abstract )
class UNREAL_POSITIONAL_API UPositionalCollider : public UActorComponent
{
	friend class APositionalWorld;

	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<APositionalWorld> World;

	UPROPERTY(EditAnywhere)
		float Density;

	UPROPERTY(EditAnywhere)
		float StaticFriction;

	UPROPERTY(EditAnywhere)
		float DynamicFriction;

	UPROPERTY(EditAnywhere)
		float Bounciness;

	UPROPERTY(EditAnywhere)
		FVector Center;

	// Sets default values for this component's properties
	UPositionalCollider();

protected:
	Ref<Collider> m_Collider;
	TSoftObjectPtr<APositionalWorld> m_PrevWorld;

	FTransform GetRelativeTransform() const;
	virtual void SyncTransform(const FTransform& transform)
	{
		UE_LOG(LogPositional, Fatal, TEXT("SyncTransform not implemented!"));
	}

	virtual Ref<Collider> CreateCollider(APositionalWorld *world, APositionalRigidBody *body, const FTransform& transform)
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
