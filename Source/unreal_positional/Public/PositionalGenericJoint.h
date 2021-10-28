// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PositionalConstraint.h"
#include "PositionalGenericJoint.generated.h"

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDOF : uint8
{
	Linear = 1      UMETA(DisplayName="Linear"),
	Planar = 1 << 1 UMETA(DisplayName="Planar"),
	Twist = 1 << 2  UMETA(DisplayName="Twist"),
	Swing = 1 << 3  UMETA(DisplayName="Swing")
};
ENUM_CLASS_FLAGS(EDOF);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalGenericJoint : public UPositionalConstraint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector AnchorPositionA;

	UPROPERTY(EditAnywhere)
	FRotator AnchorRotationA;

	UPROPERTY(EditAnywhere)
	FVector AnchorPositionB;

	UPROPERTY(EditAnywhere)
	FRotator AnchorRotationB;

	UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum = "EDOF"))
	uint8 DOF;

	UPROPERTY(EditAnywhere, meta=(Bitmask, BitmaskEnum = "EDOF"))
	uint8 HasLimits;

	UPROPERTY(EditAnywhere)
	double PositionCompliance;

	UPROPERTY(EditAnywhere)
	double PositionDamping;

	UPROPERTY(EditAnywhere)
	double LinearLimit;

	UPROPERTY(EditAnywhere)
	double RotationCompliance;

	UPROPERTY(EditAnywhere)
	double RotationDamping;

	UPROPERTY(EditAnywhere)
	double MinTwist;

	UPROPERTY(EditAnywhere)
	double MaxTwist;

	UPROPERTY(EditAnywhere)
	double MinSwing;

	UPROPERTY(EditAnywhere)
	double MaxSwing;

	// Sets default values for this component's properties
	UPositionalGenericJoint();

protected:
	virtual Ref<Constraint> CreateConstraint(APositionalWorld *world, APositionalRigidBody *bodyA, APositionalRigidBody *bodyB) override;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
