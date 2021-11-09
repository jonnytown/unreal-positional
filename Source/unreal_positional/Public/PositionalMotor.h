// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PositionalConstraint.h"
#include "PositionalMotor.generated.h"


UCLASS( ClassGroup=(Positional), meta=(BlueprintSpawnableComponent) )
class UNREAL_POSITIONAL_API UPositionalMotor : public UPositionalConstraint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Motor")
	FRotator AxisRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraint|Motor")
	double Torque;

	// Sets default values for this component's properties
	UPositionalMotor();

protected:
	virtual Ref<Constraint> CreateConstraint(Positional::World *world, const Ref<Body> &bodyA, const Ref<Body> &bodyB) override;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
