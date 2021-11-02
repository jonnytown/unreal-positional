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

UCLASS( ClassGroup=(Positional), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalGenericJoint : public UPositionalConstraint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Constraint|Anchor A")
	FVector AnchorPositionA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Anchor A")
	FRotator AnchorRotationA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Anchor B")
	FVector AnchorPositionB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Anchor B")
	FRotator AnchorRotationB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum = "EDOF"), Category = "Constraint|Joint")
	uint8 DOF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EDOF"), Category = "Constraint|Joint")
	uint8 HasLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double PositionCompliance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double PositionDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double LinearLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double RotationCompliance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double RotationDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double MinTwist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double MaxTwist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double MinSwing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Joint")
	double MaxSwing;

	// Sets default values for this component's properties
	UPositionalGenericJoint();

protected:
	virtual Ref<Constraint> CreateConstraint(Positional::World *world, const Ref<Body> &bodyA, const Ref<Body> &bodyB) override;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
