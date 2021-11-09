// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionalMotor.h"
#include "constraints/MotorConstraint.h"
#include "PositionalUtil.h"
// Sets default values for this component's properties
UPositionalMotor::UPositionalMotor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

Ref<Constraint> UPositionalMotor::CreateConstraint(Positional::World *world, const Ref<Body> &bodyA, const Ref<Body> &bodyB)
{
	return world->createConstraint<MotorConstraint, MotorConstraint::Data>(
		bodyA,
		bodyB,
		IgnoreCollisions,
		ToQuat(AxisRotation),
		Torque);
}

#if WITH_EDITOR
void UPositionalMotor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString name;
	PropertyChangedEvent.MemberProperty->GetName(name);
	if (m_Constraint.valid())
	{
		auto data = m_Constraint.get().getData<MotorConstraint::Data>();

		if (name == TEXT("AxisRotation"))
		{
			data->rotation = ToQuat(AxisRotation);
		}
	}
}
#endif //WITH_EDITOR
